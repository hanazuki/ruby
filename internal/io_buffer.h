#pragma once

#include <ruby/ruby.h>
#include <ruby/io/buffer.h>

RUBY_SYMBOL_EXPORT_BEGIN

/**
 * Wrap string_or_buffer as a read-only IO::Buffer view and invoke callback(buffer, argument).
 *
 * The resulting buffer's backing allocation is locked for the duration of the
 * callback and automatically unlocked when the callback returns or raises.
 *
 * - IO::Buffer: locked and passed directly to the callback.
 * - String: locked to prevent GC compaction from moving the backing memory,
 *   wrapped in a locked read-only IO::Buffer, callback called inside
 *   rb_ensure, buffer freed and string unlocked on exit.
 * - Other: TypeError raised.
 */
VALUE rb_io_buffer_for_reading(VALUE string_or_buffer, VALUE (*callback)(VALUE buffer, VALUE argument), VALUE argument);

/**
 * Wrap string_or_buffer as a writable IO::Buffer view and invoke callback(buffer, argument).
 *
 * - Read-only IO::Buffer: ArgumentError raised.
 * The resulting buffer's backing allocation is locked for the duration of the
 * callback and automatically unlocked when the callback returns or raises.
 *
 * - IO::Buffer: locked and passed directly to the callback.
 * - String: locked, wrapped in a writable IO::Buffer, callback called inside
 *   rb_ensure, buffer unlocked and freed, and string unlocked on exit.
 * - Other: TypeError raised.
 */
VALUE rb_io_buffer_for_writing(VALUE string_or_buffer, VALUE (*callback)(VALUE buffer, VALUE argument), VALUE argument);

RUBY_SYMBOL_EXPORT_END

#if USE_YJIT
/**
 * Read a Fixnum value of the specified signedness and width from the buffer.
 *
 * The caller must ensure that buffer is an instance of IO::Buffer.
 * Given that precondition, these functions do not raise: they return Qundef
 * when the buffer is invalid or the requested value is out of bounds.
 */
VALUE rb_io_buffer_try_read_U8(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_S8(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_U16(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_u16(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_S16(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_s16(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_U32(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_u32(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_S32(VALUE buffer, size_t offset);
VALUE rb_io_buffer_try_read_s32(VALUE buffer, size_t offset);
#endif
