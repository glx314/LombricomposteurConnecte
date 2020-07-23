# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: lorasender.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='lorasender.proto',
  package='lorasender',
  syntax='proto2',
  serialized_options=None,
  serialized_pb=b'\n\x10lorasender.proto\x12\nlorasender\"p\n\x07request\x12\x31\n\x07\x63ommand\x18\x01 \x02(\x0e\x32 .lorasender.request.command_type\x12\x0c\n\x04\x64\x61ta\x18\x02 \x01(\x0c\"$\n\x0c\x63ommand_type\x12\n\n\x06STATUS\x10\x00\x12\x08\n\x04SEND\x10\x01\"\x83\x01\n\x05reply\x12\r\n\x05msgid\x18\x01 \x02(\x05\x12*\n\x04type\x18\x02 \x02(\x0e\x32\x1c.lorasender.reply.reply_type\x12\x0f\n\x07\x64\x65tails\x18\x03 \x01(\t\".\n\nreply_type\x12\x07\n\x03\x41\x43K\x10\x00\x12\x0b\n\x07SUCCESS\x10\x01\x12\n\n\x06\x46\x41ILED\x10\x02'
)



_REQUEST_COMMAND_TYPE = _descriptor.EnumDescriptor(
  name='command_type',
  full_name='lorasender.request.command_type',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='STATUS', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SEND', index=1, number=1,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=108,
  serialized_end=144,
)
_sym_db.RegisterEnumDescriptor(_REQUEST_COMMAND_TYPE)

_REPLY_REPLY_TYPE = _descriptor.EnumDescriptor(
  name='reply_type',
  full_name='lorasender.reply.reply_type',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='ACK', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SUCCESS', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='FAILED', index=2, number=2,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=232,
  serialized_end=278,
)
_sym_db.RegisterEnumDescriptor(_REPLY_REPLY_TYPE)


_REQUEST = _descriptor.Descriptor(
  name='request',
  full_name='lorasender.request',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='command', full_name='lorasender.request.command', index=0,
      number=1, type=14, cpp_type=8, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='data', full_name='lorasender.request.data', index=1,
      number=2, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _REQUEST_COMMAND_TYPE,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=32,
  serialized_end=144,
)


_REPLY = _descriptor.Descriptor(
  name='reply',
  full_name='lorasender.reply',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='msgid', full_name='lorasender.reply.msgid', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='type', full_name='lorasender.reply.type', index=1,
      number=2, type=14, cpp_type=8, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='details', full_name='lorasender.reply.details', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _REPLY_REPLY_TYPE,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=147,
  serialized_end=278,
)

_REQUEST.fields_by_name['command'].enum_type = _REQUEST_COMMAND_TYPE
_REQUEST_COMMAND_TYPE.containing_type = _REQUEST
_REPLY.fields_by_name['type'].enum_type = _REPLY_REPLY_TYPE
_REPLY_REPLY_TYPE.containing_type = _REPLY
DESCRIPTOR.message_types_by_name['request'] = _REQUEST
DESCRIPTOR.message_types_by_name['reply'] = _REPLY
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

request = _reflection.GeneratedProtocolMessageType('request', (_message.Message,), {
  'DESCRIPTOR' : _REQUEST,
  '__module__' : 'lorasender_pb2'
  # @@protoc_insertion_point(class_scope:lorasender.request)
  })
_sym_db.RegisterMessage(request)

reply = _reflection.GeneratedProtocolMessageType('reply', (_message.Message,), {
  'DESCRIPTOR' : _REPLY,
  '__module__' : 'lorasender_pb2'
  # @@protoc_insertion_point(class_scope:lorasender.reply)
  })
_sym_db.RegisterMessage(reply)


# @@protoc_insertion_point(module_scope)
