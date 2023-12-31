// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: syncMessage.proto

#ifndef PROTOBUF_INCLUDED_syncMessage_2eproto
#define PROTOBUF_INCLUDED_syncMessage_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_syncMessage_2eproto 

namespace protobuf_syncMessage_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[4];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_syncMessage_2eproto
namespace TCCamp {
class Float3;
class Float3DefaultTypeInternal;
extern Float3DefaultTypeInternal _Float3_default_instance_;
class Float4;
class Float4DefaultTypeInternal;
extern Float4DefaultTypeInternal _Float4_default_instance_;
class PlayerSyncInfo;
class PlayerSyncInfoDefaultTypeInternal;
extern PlayerSyncInfoDefaultTypeInternal _PlayerSyncInfo_default_instance_;
class ServerSyncInfo;
class ServerSyncInfoDefaultTypeInternal;
extern ServerSyncInfoDefaultTypeInternal _ServerSyncInfo_default_instance_;
}  // namespace TCCamp
namespace google {
namespace protobuf {
template<> ::TCCamp::Float3* Arena::CreateMaybeMessage<::TCCamp::Float3>(Arena*);
template<> ::TCCamp::Float4* Arena::CreateMaybeMessage<::TCCamp::Float4>(Arena*);
template<> ::TCCamp::PlayerSyncInfo* Arena::CreateMaybeMessage<::TCCamp::PlayerSyncInfo>(Arena*);
template<> ::TCCamp::ServerSyncInfo* Arena::CreateMaybeMessage<::TCCamp::ServerSyncInfo>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace TCCamp {

enum CLIENT_SYNC_CMD {
  CLIENT_INVALID = 0,
  CLIENT_SYNC_DATA_SEND = 31001,
  CLIENT_SYNC_CMD_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  CLIENT_SYNC_CMD_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool CLIENT_SYNC_CMD_IsValid(int value);
const CLIENT_SYNC_CMD CLIENT_SYNC_CMD_MIN = CLIENT_INVALID;
const CLIENT_SYNC_CMD CLIENT_SYNC_CMD_MAX = CLIENT_SYNC_DATA_SEND;
const int CLIENT_SYNC_CMD_ARRAYSIZE = CLIENT_SYNC_CMD_MAX + 1;

const ::google::protobuf::EnumDescriptor* CLIENT_SYNC_CMD_descriptor();
inline const ::std::string& CLIENT_SYNC_CMD_Name(CLIENT_SYNC_CMD value) {
  return ::google::protobuf::internal::NameOfEnum(
    CLIENT_SYNC_CMD_descriptor(), value);
}
inline bool CLIENT_SYNC_CMD_Parse(
    const ::std::string& name, CLIENT_SYNC_CMD* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CLIENT_SYNC_CMD>(
    CLIENT_SYNC_CMD_descriptor(), name, value);
}
enum SERVER_SYNC_CMD {
  SERVER_INVALID = 0,
  SERVER_SYNC_DATA_SEND = 1201,
  SERVER_NEW_PLAYER_LOGIN = 1203,
  SERVER_PLAYER_LOGOUT = 1204,
  SERVER_SYNC_CMD_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SERVER_SYNC_CMD_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SERVER_SYNC_CMD_IsValid(int value);
const SERVER_SYNC_CMD SERVER_SYNC_CMD_MIN = SERVER_INVALID;
const SERVER_SYNC_CMD SERVER_SYNC_CMD_MAX = SERVER_PLAYER_LOGOUT;
const int SERVER_SYNC_CMD_ARRAYSIZE = SERVER_SYNC_CMD_MAX + 1;

const ::google::protobuf::EnumDescriptor* SERVER_SYNC_CMD_descriptor();
inline const ::std::string& SERVER_SYNC_CMD_Name(SERVER_SYNC_CMD value) {
  return ::google::protobuf::internal::NameOfEnum(
    SERVER_SYNC_CMD_descriptor(), value);
}
inline bool SERVER_SYNC_CMD_Parse(
    const ::std::string& name, SERVER_SYNC_CMD* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SERVER_SYNC_CMD>(
    SERVER_SYNC_CMD_descriptor(), name, value);
}
// ===================================================================

class Float3 : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TCCamp.Float3) */ {
 public:
  Float3();
  virtual ~Float3();

  Float3(const Float3& from);

  inline Float3& operator=(const Float3& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Float3(Float3&& from) noexcept
    : Float3() {
    *this = ::std::move(from);
  }

  inline Float3& operator=(Float3&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Float3& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Float3* internal_default_instance() {
    return reinterpret_cast<const Float3*>(
               &_Float3_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Float3* other);
  friend void swap(Float3& a, Float3& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Float3* New() const final {
    return CreateMaybeMessage<Float3>(NULL);
  }

  Float3* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Float3>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Float3& from);
  void MergeFrom(const Float3& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Float3* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // float x = 1;
  void clear_x();
  static const int kXFieldNumber = 1;
  float x() const;
  void set_x(float value);

  // float y = 2;
  void clear_y();
  static const int kYFieldNumber = 2;
  float y() const;
  void set_y(float value);

  // float z = 3;
  void clear_z();
  static const int kZFieldNumber = 3;
  float z() const;
  void set_z(float value);

  // @@protoc_insertion_point(class_scope:TCCamp.Float3)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  float x_;
  float y_;
  float z_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_syncMessage_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Float4 : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TCCamp.Float4) */ {
 public:
  Float4();
  virtual ~Float4();

  Float4(const Float4& from);

  inline Float4& operator=(const Float4& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Float4(Float4&& from) noexcept
    : Float4() {
    *this = ::std::move(from);
  }

  inline Float4& operator=(Float4&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Float4& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Float4* internal_default_instance() {
    return reinterpret_cast<const Float4*>(
               &_Float4_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Float4* other);
  friend void swap(Float4& a, Float4& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Float4* New() const final {
    return CreateMaybeMessage<Float4>(NULL);
  }

  Float4* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Float4>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Float4& from);
  void MergeFrom(const Float4& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Float4* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // float x = 1;
  void clear_x();
  static const int kXFieldNumber = 1;
  float x() const;
  void set_x(float value);

  // float y = 2;
  void clear_y();
  static const int kYFieldNumber = 2;
  float y() const;
  void set_y(float value);

  // float z = 3;
  void clear_z();
  static const int kZFieldNumber = 3;
  float z() const;
  void set_z(float value);

  // float w = 4;
  void clear_w();
  static const int kWFieldNumber = 4;
  float w() const;
  void set_w(float value);

  // @@protoc_insertion_point(class_scope:TCCamp.Float4)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  float x_;
  float y_;
  float z_;
  float w_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_syncMessage_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class PlayerSyncInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TCCamp.PlayerSyncInfo) */ {
 public:
  PlayerSyncInfo();
  virtual ~PlayerSyncInfo();

  PlayerSyncInfo(const PlayerSyncInfo& from);

  inline PlayerSyncInfo& operator=(const PlayerSyncInfo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PlayerSyncInfo(PlayerSyncInfo&& from) noexcept
    : PlayerSyncInfo() {
    *this = ::std::move(from);
  }

  inline PlayerSyncInfo& operator=(PlayerSyncInfo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const PlayerSyncInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PlayerSyncInfo* internal_default_instance() {
    return reinterpret_cast<const PlayerSyncInfo*>(
               &_PlayerSyncInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(PlayerSyncInfo* other);
  friend void swap(PlayerSyncInfo& a, PlayerSyncInfo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PlayerSyncInfo* New() const final {
    return CreateMaybeMessage<PlayerSyncInfo>(NULL);
  }

  PlayerSyncInfo* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<PlayerSyncInfo>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const PlayerSyncInfo& from);
  void MergeFrom(const PlayerSyncInfo& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PlayerSyncInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string PlayerID = 4;
  void clear_playerid();
  static const int kPlayerIDFieldNumber = 4;
  const ::std::string& playerid() const;
  void set_playerid(const ::std::string& value);
  #if LANG_CXX11
  void set_playerid(::std::string&& value);
  #endif
  void set_playerid(const char* value);
  void set_playerid(const char* value, size_t size);
  ::std::string* mutable_playerid();
  ::std::string* release_playerid();
  void set_allocated_playerid(::std::string* playerid);

  // bytes PlayerName = 5;
  void clear_playername();
  static const int kPlayerNameFieldNumber = 5;
  const ::std::string& playername() const;
  void set_playername(const ::std::string& value);
  #if LANG_CXX11
  void set_playername(::std::string&& value);
  #endif
  void set_playername(const char* value);
  void set_playername(const void* value, size_t size);
  ::std::string* mutable_playername();
  ::std::string* release_playername();
  void set_allocated_playername(::std::string* playername);

  // .TCCamp.Float3 Position = 1;
  bool has_position() const;
  void clear_position();
  static const int kPositionFieldNumber = 1;
  private:
  const ::TCCamp::Float3& _internal_position() const;
  public:
  const ::TCCamp::Float3& position() const;
  ::TCCamp::Float3* release_position();
  ::TCCamp::Float3* mutable_position();
  void set_allocated_position(::TCCamp::Float3* position);

  // .TCCamp.Float4 Rotation = 2;
  bool has_rotation() const;
  void clear_rotation();
  static const int kRotationFieldNumber = 2;
  private:
  const ::TCCamp::Float4& _internal_rotation() const;
  public:
  const ::TCCamp::Float4& rotation() const;
  ::TCCamp::Float4* release_rotation();
  ::TCCamp::Float4* mutable_rotation();
  void set_allocated_rotation(::TCCamp::Float4* rotation);

  // float Speed = 3;
  void clear_speed();
  static const int kSpeedFieldNumber = 3;
  float speed() const;
  void set_speed(float value);

  // @@protoc_insertion_point(class_scope:TCCamp.PlayerSyncInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr playerid_;
  ::google::protobuf::internal::ArenaStringPtr playername_;
  ::TCCamp::Float3* position_;
  ::TCCamp::Float4* rotation_;
  float speed_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_syncMessage_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class ServerSyncInfo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TCCamp.ServerSyncInfo) */ {
 public:
  ServerSyncInfo();
  virtual ~ServerSyncInfo();

  ServerSyncInfo(const ServerSyncInfo& from);

  inline ServerSyncInfo& operator=(const ServerSyncInfo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ServerSyncInfo(ServerSyncInfo&& from) noexcept
    : ServerSyncInfo() {
    *this = ::std::move(from);
  }

  inline ServerSyncInfo& operator=(ServerSyncInfo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const ServerSyncInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ServerSyncInfo* internal_default_instance() {
    return reinterpret_cast<const ServerSyncInfo*>(
               &_ServerSyncInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  void Swap(ServerSyncInfo* other);
  friend void swap(ServerSyncInfo& a, ServerSyncInfo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ServerSyncInfo* New() const final {
    return CreateMaybeMessage<ServerSyncInfo>(NULL);
  }

  ServerSyncInfo* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ServerSyncInfo>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const ServerSyncInfo& from);
  void MergeFrom(const ServerSyncInfo& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ServerSyncInfo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .TCCamp.PlayerSyncInfo Players = 1;
  int players_size() const;
  void clear_players();
  static const int kPlayersFieldNumber = 1;
  ::TCCamp::PlayerSyncInfo* mutable_players(int index);
  ::google::protobuf::RepeatedPtrField< ::TCCamp::PlayerSyncInfo >*
      mutable_players();
  const ::TCCamp::PlayerSyncInfo& players(int index) const;
  ::TCCamp::PlayerSyncInfo* add_players();
  const ::google::protobuf::RepeatedPtrField< ::TCCamp::PlayerSyncInfo >&
      players() const;

  // @@protoc_insertion_point(class_scope:TCCamp.ServerSyncInfo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::TCCamp::PlayerSyncInfo > players_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_syncMessage_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Float3

// float x = 1;
inline void Float3::clear_x() {
  x_ = 0;
}
inline float Float3::x() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float3.x)
  return x_;
}
inline void Float3::set_x(float value) {
  
  x_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float3.x)
}

// float y = 2;
inline void Float3::clear_y() {
  y_ = 0;
}
inline float Float3::y() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float3.y)
  return y_;
}
inline void Float3::set_y(float value) {
  
  y_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float3.y)
}

// float z = 3;
inline void Float3::clear_z() {
  z_ = 0;
}
inline float Float3::z() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float3.z)
  return z_;
}
inline void Float3::set_z(float value) {
  
  z_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float3.z)
}

// -------------------------------------------------------------------

// Float4

// float x = 1;
inline void Float4::clear_x() {
  x_ = 0;
}
inline float Float4::x() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float4.x)
  return x_;
}
inline void Float4::set_x(float value) {
  
  x_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float4.x)
}

// float y = 2;
inline void Float4::clear_y() {
  y_ = 0;
}
inline float Float4::y() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float4.y)
  return y_;
}
inline void Float4::set_y(float value) {
  
  y_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float4.y)
}

// float z = 3;
inline void Float4::clear_z() {
  z_ = 0;
}
inline float Float4::z() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float4.z)
  return z_;
}
inline void Float4::set_z(float value) {
  
  z_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float4.z)
}

// float w = 4;
inline void Float4::clear_w() {
  w_ = 0;
}
inline float Float4::w() const {
  // @@protoc_insertion_point(field_get:TCCamp.Float4.w)
  return w_;
}
inline void Float4::set_w(float value) {
  
  w_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.Float4.w)
}

// -------------------------------------------------------------------

// PlayerSyncInfo

// .TCCamp.Float3 Position = 1;
inline bool PlayerSyncInfo::has_position() const {
  return this != internal_default_instance() && position_ != NULL;
}
inline void PlayerSyncInfo::clear_position() {
  if (GetArenaNoVirtual() == NULL && position_ != NULL) {
    delete position_;
  }
  position_ = NULL;
}
inline const ::TCCamp::Float3& PlayerSyncInfo::_internal_position() const {
  return *position_;
}
inline const ::TCCamp::Float3& PlayerSyncInfo::position() const {
  const ::TCCamp::Float3* p = position_;
  // @@protoc_insertion_point(field_get:TCCamp.PlayerSyncInfo.Position)
  return p != NULL ? *p : *reinterpret_cast<const ::TCCamp::Float3*>(
      &::TCCamp::_Float3_default_instance_);
}
inline ::TCCamp::Float3* PlayerSyncInfo::release_position() {
  // @@protoc_insertion_point(field_release:TCCamp.PlayerSyncInfo.Position)
  
  ::TCCamp::Float3* temp = position_;
  position_ = NULL;
  return temp;
}
inline ::TCCamp::Float3* PlayerSyncInfo::mutable_position() {
  
  if (position_ == NULL) {
    auto* p = CreateMaybeMessage<::TCCamp::Float3>(GetArenaNoVirtual());
    position_ = p;
  }
  // @@protoc_insertion_point(field_mutable:TCCamp.PlayerSyncInfo.Position)
  return position_;
}
inline void PlayerSyncInfo::set_allocated_position(::TCCamp::Float3* position) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete position_;
  }
  if (position) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      position = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, position, submessage_arena);
    }
    
  } else {
    
  }
  position_ = position;
  // @@protoc_insertion_point(field_set_allocated:TCCamp.PlayerSyncInfo.Position)
}

// .TCCamp.Float4 Rotation = 2;
inline bool PlayerSyncInfo::has_rotation() const {
  return this != internal_default_instance() && rotation_ != NULL;
}
inline void PlayerSyncInfo::clear_rotation() {
  if (GetArenaNoVirtual() == NULL && rotation_ != NULL) {
    delete rotation_;
  }
  rotation_ = NULL;
}
inline const ::TCCamp::Float4& PlayerSyncInfo::_internal_rotation() const {
  return *rotation_;
}
inline const ::TCCamp::Float4& PlayerSyncInfo::rotation() const {
  const ::TCCamp::Float4* p = rotation_;
  // @@protoc_insertion_point(field_get:TCCamp.PlayerSyncInfo.Rotation)
  return p != NULL ? *p : *reinterpret_cast<const ::TCCamp::Float4*>(
      &::TCCamp::_Float4_default_instance_);
}
inline ::TCCamp::Float4* PlayerSyncInfo::release_rotation() {
  // @@protoc_insertion_point(field_release:TCCamp.PlayerSyncInfo.Rotation)
  
  ::TCCamp::Float4* temp = rotation_;
  rotation_ = NULL;
  return temp;
}
inline ::TCCamp::Float4* PlayerSyncInfo::mutable_rotation() {
  
  if (rotation_ == NULL) {
    auto* p = CreateMaybeMessage<::TCCamp::Float4>(GetArenaNoVirtual());
    rotation_ = p;
  }
  // @@protoc_insertion_point(field_mutable:TCCamp.PlayerSyncInfo.Rotation)
  return rotation_;
}
inline void PlayerSyncInfo::set_allocated_rotation(::TCCamp::Float4* rotation) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete rotation_;
  }
  if (rotation) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      rotation = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, rotation, submessage_arena);
    }
    
  } else {
    
  }
  rotation_ = rotation;
  // @@protoc_insertion_point(field_set_allocated:TCCamp.PlayerSyncInfo.Rotation)
}

// float Speed = 3;
inline void PlayerSyncInfo::clear_speed() {
  speed_ = 0;
}
inline float PlayerSyncInfo::speed() const {
  // @@protoc_insertion_point(field_get:TCCamp.PlayerSyncInfo.Speed)
  return speed_;
}
inline void PlayerSyncInfo::set_speed(float value) {
  
  speed_ = value;
  // @@protoc_insertion_point(field_set:TCCamp.PlayerSyncInfo.Speed)
}

// string PlayerID = 4;
inline void PlayerSyncInfo::clear_playerid() {
  playerid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PlayerSyncInfo::playerid() const {
  // @@protoc_insertion_point(field_get:TCCamp.PlayerSyncInfo.PlayerID)
  return playerid_.GetNoArena();
}
inline void PlayerSyncInfo::set_playerid(const ::std::string& value) {
  
  playerid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TCCamp.PlayerSyncInfo.PlayerID)
}
#if LANG_CXX11
inline void PlayerSyncInfo::set_playerid(::std::string&& value) {
  
  playerid_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:TCCamp.PlayerSyncInfo.PlayerID)
}
#endif
inline void PlayerSyncInfo::set_playerid(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  playerid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TCCamp.PlayerSyncInfo.PlayerID)
}
inline void PlayerSyncInfo::set_playerid(const char* value, size_t size) {
  
  playerid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TCCamp.PlayerSyncInfo.PlayerID)
}
inline ::std::string* PlayerSyncInfo::mutable_playerid() {
  
  // @@protoc_insertion_point(field_mutable:TCCamp.PlayerSyncInfo.PlayerID)
  return playerid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PlayerSyncInfo::release_playerid() {
  // @@protoc_insertion_point(field_release:TCCamp.PlayerSyncInfo.PlayerID)
  
  return playerid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PlayerSyncInfo::set_allocated_playerid(::std::string* playerid) {
  if (playerid != NULL) {
    
  } else {
    
  }
  playerid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), playerid);
  // @@protoc_insertion_point(field_set_allocated:TCCamp.PlayerSyncInfo.PlayerID)
}

// bytes PlayerName = 5;
inline void PlayerSyncInfo::clear_playername() {
  playername_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PlayerSyncInfo::playername() const {
  // @@protoc_insertion_point(field_get:TCCamp.PlayerSyncInfo.PlayerName)
  return playername_.GetNoArena();
}
inline void PlayerSyncInfo::set_playername(const ::std::string& value) {
  
  playername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TCCamp.PlayerSyncInfo.PlayerName)
}
#if LANG_CXX11
inline void PlayerSyncInfo::set_playername(::std::string&& value) {
  
  playername_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:TCCamp.PlayerSyncInfo.PlayerName)
}
#endif
inline void PlayerSyncInfo::set_playername(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  playername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TCCamp.PlayerSyncInfo.PlayerName)
}
inline void PlayerSyncInfo::set_playername(const void* value, size_t size) {
  
  playername_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TCCamp.PlayerSyncInfo.PlayerName)
}
inline ::std::string* PlayerSyncInfo::mutable_playername() {
  
  // @@protoc_insertion_point(field_mutable:TCCamp.PlayerSyncInfo.PlayerName)
  return playername_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PlayerSyncInfo::release_playername() {
  // @@protoc_insertion_point(field_release:TCCamp.PlayerSyncInfo.PlayerName)
  
  return playername_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PlayerSyncInfo::set_allocated_playername(::std::string* playername) {
  if (playername != NULL) {
    
  } else {
    
  }
  playername_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), playername);
  // @@protoc_insertion_point(field_set_allocated:TCCamp.PlayerSyncInfo.PlayerName)
}

// -------------------------------------------------------------------

// ServerSyncInfo

// repeated .TCCamp.PlayerSyncInfo Players = 1;
inline int ServerSyncInfo::players_size() const {
  return players_.size();
}
inline void ServerSyncInfo::clear_players() {
  players_.Clear();
}
inline ::TCCamp::PlayerSyncInfo* ServerSyncInfo::mutable_players(int index) {
  // @@protoc_insertion_point(field_mutable:TCCamp.ServerSyncInfo.Players)
  return players_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::TCCamp::PlayerSyncInfo >*
ServerSyncInfo::mutable_players() {
  // @@protoc_insertion_point(field_mutable_list:TCCamp.ServerSyncInfo.Players)
  return &players_;
}
inline const ::TCCamp::PlayerSyncInfo& ServerSyncInfo::players(int index) const {
  // @@protoc_insertion_point(field_get:TCCamp.ServerSyncInfo.Players)
  return players_.Get(index);
}
inline ::TCCamp::PlayerSyncInfo* ServerSyncInfo::add_players() {
  // @@protoc_insertion_point(field_add:TCCamp.ServerSyncInfo.Players)
  return players_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::TCCamp::PlayerSyncInfo >&
ServerSyncInfo::players() const {
  // @@protoc_insertion_point(field_list:TCCamp.ServerSyncInfo.Players)
  return players_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace TCCamp

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::TCCamp::CLIENT_SYNC_CMD> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::TCCamp::CLIENT_SYNC_CMD>() {
  return ::TCCamp::CLIENT_SYNC_CMD_descriptor();
}
template <> struct is_proto_enum< ::TCCamp::SERVER_SYNC_CMD> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::TCCamp::SERVER_SYNC_CMD>() {
  return ::TCCamp::SERVER_SYNC_CMD_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_syncMessage_2eproto
