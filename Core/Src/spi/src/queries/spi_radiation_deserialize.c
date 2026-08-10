#include "queries/spi_radiation_generated.h"

#warning "implement ACKNOWLEDGE (query 0x01) (delete line once done)"
bool deserialize_radiation_acknowledge(radiation_acknowledge_outputs_t *outputs, const uint8_t *buffer,
                                       uint16_t buffer_len) {
    // TODO: implement — ACKNOWLEDGE (query 0x01)
    // this must take buffer and unpack into radiation_acknowledge_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;
}
#warning "implement ECHO (query 0x02) (delete line once done)"
// notes: echo query
bool deserialize_radiation_echo(radiation_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    // TODO: implement — ECHO (query 0x02)
    // this must take buffer and unpack into radiation_echo_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;
}
#warning "implement EVENT_TIMESTAMPS (query 0x03) (delete line once done)"
// notes: dequeues up to 10 timestamps
bool deserialize_radiation_event_timestamps(radiation_event_timestamps_outputs_t *outputs, const uint8_t *buffer,
                                            uint16_t buffer_len) {
    // TODO: implement — EVENT_TIMESTAMPS (query 0x03)
    // this must take buffer and unpack into radiation_event_timestamps_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;
}
#warning "implement ADC_VALUE (query 0x04) (delete line once done)"
bool deserialize_radiation_adc_value(radiation_adc_value_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    // TODO: implement — ADC_VALUE (query 0x04)
    // this must take buffer and unpack into radiation_adc_value_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;
}