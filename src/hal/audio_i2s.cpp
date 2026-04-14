// I2S engine — PIO + DMA + IRQ glue for output, input, and full-duplex.
//
// PHASE 0: file reserved. Phase 1 brings the PIO programs and the runtime
// sample-rate / bit-depth machinery here. Until then audio.cpp owns the
// user-facing API and the engine is a no-op.
