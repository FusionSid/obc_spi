from dataclasses import dataclass

from datatypes import PRIM_SIZES, Array, Empty, Payload, Struct, parse_payload


@dataclass
class Query:
    payload: str
    query_code: int
    command: str
    inputs: Payload
    outputs: Payload
    notes: str

    @classmethod
    def from_csv_row(cls, row: dict):
        return cls(
            payload=row["payload"].strip().upper(),
            query_code=int(row["query_code"].strip(), 0),
            command=row["command"].strip().upper(),
            inputs=parse_payload(row.get("inputs", "")),
            outputs=parse_payload(row.get("outputs", "")),
            notes=row.get("notes", "").strip(),
        )

    @property
    def prefix(self) -> str:
        return f"{self.payload.lower()}_{self.command.lower()}"

    @property
    def function_name(self) -> str:
        return f"{self.payload.lower()}_query_{self.command.lower()}"

    @property
    def query_enum(self) -> str:
        return f"SPI_{self.payload}_QUERY_{self.command}"

    @property
    def inputs_type(self) -> str | None:
        return None if isinstance(self.inputs, Empty) else f"{self.prefix}_inputs_t"

    @property
    def outputs_type(self) -> str | None:
        return None if isinstance(self.outputs, Empty) else f"{self.prefix}_outputs_t"

    @property
    def serialize_fn(self) -> str | None:
        return None if isinstance(self.inputs, Empty) else f"serialize_{self.prefix}"

    @property
    def deserialize_fn(self) -> str | None:
        return None if isinstance(self.outputs, Empty) else f"deserialize_{self.prefix}"

    @property
    def expects_response(self) -> bool:
        return not (isinstance(self.outputs, Empty) and self.outputs.no_response)

    def _pack_bytes(self, value_expr: str, primitive: str) -> list[str]:
        if (size := PRIM_SIZES[primitive]) == 1:
            return [f"(uint8_t)({value_expr})"]
        unsigned_type = f"uint{size * 8}_t"
        base = f"({unsigned_type})({value_expr})"
        return [f"(uint8_t)(({base}) >> {pos * 8})" for pos in range(size)]

    def serialize_body(self) -> list[str]:
        lines = []

        if isinstance(self.inputs, Struct):
            lines.append("uint16_t offset = 0;")
            for f in self.inputs.fields:
                for idx in range(f.length):
                    value_expr = (
                        f"inputs->{f.name}"
                        if f.length == 1
                        else f"inputs->{f.name}[{idx}]"
                    )
                    for byte_expr in self._pack_bytes(value_expr, f.primitive_type):
                        lines.append(f"buffer[offset++] = {byte_expr};")
            lines.append("*buffer_len = offset;")

        elif isinstance(self.inputs, Array):
            lines.append("uint16_t offset = 0;")
            lines.append("for (uint16_t i = 0; i < inputs->dataLen; i++) {")
            for byte_expr in self._pack_bytes(
                f"inputs->{self.inputs.name}[i]", self.inputs.primitive_type
            ):
                lines.append(f"    buffer[offset++] = {byte_expr};")
            lines.append("}")
            lines.append("*buffer_len = offset;")

        return lines

    def default_deserialize_body(self) -> str:
        lines = [
            f"// TODO: implement - {self.command} (query 0x{self.query_code:02X})",
            f"// this must take buffer and unpack into {self.outputs_type}.",
            "(void)outputs;",
            "(void)buffer;",
            "(void)buffer_len;",
            "return false;",
        ]
        return "\n".join("    " + line for line in lines)
