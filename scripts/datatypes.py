import re
from dataclasses import dataclass

PRIM_SIZES = {"u8": 1, "i8": 1, "u16": 2, "i16": 2, "u32": 4, "i32": 4, "bool": 1}
PRIM_C_TYPES = {
    "u8": "uint8_t",
    "i8": "int8_t",
    "u16": "uint16_t",
    "i16": "int16_t",
    "u32": "uint32_t",
    "i32": "int32_t",
    "bool": "bool",
}

_PRIM_ALT = r"(?:[ui](?:8|16|32)|bool)"
_FIELD_RE = re.compile(rf"^({_PRIM_ALT})(?:\[(\d+)\])?\s+([a-zA-Z_][a-zA-Z0-9_]*)$")
_ARRAY_RE = re.compile(rf"^({_PRIM_ALT})\[0\.\.=(\d+)\]\s+([a-zA-Z_][a-zA-Z0-9_]*)$")


@dataclass
class FieldDef:
    name: str
    primitive_type: str
    length: int = 1

    @property
    def c_type(self) -> str:
        return PRIM_C_TYPES[self.primitive_type]

    def as_c_struct_field(self) -> str:
        suffix = "" if self.length == 1 else f"[{self.length}]"
        return f"{self.c_type} {self.name}{suffix};"


@dataclass
class Struct:
    fields: list[FieldDef]


@dataclass
class Array:
    name: str
    primitive_type: str
    max_length: int

    @property
    def c_type(self) -> str:
        return PRIM_C_TYPES[self.primitive_type]


@dataclass
class Empty:
    no_response: bool = False


Payload = Struct | Array | Empty


def parse_payload(v: str) -> Payload:
    v = v.strip()

    if not v or v in ("None", "NoResponse", "Nothing", "Nope", "Empty"):
        return Empty(no_response=(v == "NoResponse"))

    lines = [l.strip() for l in v.splitlines() if l.strip()]

    if len(lines) == 1:
        m = _ARRAY_RE.match(lines[0])
        if m:
            prim, max_len, name = m.groups()
            return Array(name=name, primitive_type=prim, max_length=int(max_len))

    fields = []
    for line in lines:
        m = _FIELD_RE.match(line)
        if not m:
            raise ValueError(f"invalid field given: {line}")
        prim, length, name = m.groups()
        fields.append(FieldDef(name=name, primitive_type=prim, length=int(length or 1)))
    return Struct(fields=fields)


def payload_c_struct_lines(p: Payload) -> list[str]:
    indent = 4 * " "
    if isinstance(p, Struct):
        return [indent + f.as_c_struct_field() for f in p.fields]
    if isinstance(p, Array):
        return [
            indent + "uint16_t dataLen;",
            indent + f"{p.c_type} {p.name}[{p.max_length}];",
        ]
    return []
