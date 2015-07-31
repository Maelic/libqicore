/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/

#include <qicore/file.hpp>

#include <boost/filesystem/fstream.hpp>
#include <algorithm>
#include <vector>

#include <qi/anymodule.hpp>

namespace qi
{
class FileImpl : public File
{
public:
  FileImpl(const Path& localFilePath)
  {
    if (!localFilePath.exists())
    {
      throw std::runtime_error("File not found.");
    }

    _progressNotifier = createProgressNotifier();

    _fileStream.open(localFilePath.bfsPath(), std::ios::in | std::ios::binary);
    if (_fileStream.is_open())
    {
      _fileStream.seekg(0, _fileStream.end);
      _size = _fileStream.tellg();
      _fileStream.seekg(0, _fileStream.beg);
      assert(_fileStream.tellg() == std::streamoff(0));
    }
  }

  ~FileImpl()
  {
  }

  Buffer _read(std::streamoff beginOffset, std::streamsize countBytesToRead)
  {
    if (_seek(beginOffset))
      return _read(countBytesToRead);
    else
      return Buffer();
  }

  Buffer _read(std::streamsize countBytesToRead)
  {
    requireOpenFile();
    if (countBytesToRead > MAX_READ_SIZE)
      throw std::runtime_error("Tried to read too much data at once.");

    Buffer output;

    assert(_fileStream.is_open());
    const std::streamoff initialCursorPos = static_cast<std::streamoff>(_fileStream.tellg());
    const std::streamoff targetEnd = std::min(initialCursorPos + countBytesToRead, static_cast<std::streamoff>(_size));
    const std::streamsize distanceToTargetEnd = targetEnd - initialCursorPos;
    const std::streamsize byteCountToRead = std::min(static_cast<std::streamsize>(MAX_READ_SIZE), distanceToTargetEnd);
    assert(byteCountToRead <= MAX_READ_SIZE);

    _readBuffer.resize(byteCountToRead, '\0');
    _fileStream.read(_readBuffer.data(), byteCountToRead);
    const std::streamsize bytesRead = _fileStream.gcount();
    assert(bytesRead <= byteCountToRead);
    output.write(_readBuffer.data(), bytesRead);

    return output;
  }

  bool _seek(std::streamoff offsetFromBegin)
  {
    requireOpenFile();

    if (offsetFromBegin >= _size)
      return false;

    _fileStream.seekg(offsetFromBegin);
    return true;
  }

  void _close()
  {
    _fileStream.close();
    _size = 0;
  }

  std::streamsize size() const
  {
    return _size;
  }

  bool isOpen() const
  {
    return _fileStream.is_open();
  }

  bool isRemote() const
  {
    return false;
  }

  ProgressNotifierPtr operationProgress() const
  {
    return _progressNotifier;
  }

private:
  boost::filesystem::ifstream _fileStream;
  std::vector<char> _readBuffer;
  std::streamsize _size;
  ProgressNotifierPtr _progressNotifier;

  void requireOpenFile()
  {
    if (!_fileStream.is_open())
      throw std::runtime_error("Trying to manipulate a closed file access.");
  }
};

static bool _qiregisterFile()
{
  ::qi::ObjectTypeBuilder<File> builder;
  builder.advertiseMethod("_read", static_cast<Buffer (File::*)(std::streamoff, std::streamsize)>(&File::_read));
  builder.advertiseMethod("_read", static_cast<Buffer (File::*)(std::streamsize)>(&File::_read));
  builder.advertiseMethod("_seek", &File::_seek);
  builder.advertiseMethod("_close", &File::_close);
  builder.advertiseMethod("size", &File::size);
  builder.advertiseMethod("isOpen", &File::isOpen);
  builder.advertiseMethod("isLocal", &File::isRemote);
  builder.advertiseMethod("operationProgress", &File::operationProgress);

  builder.registerType();
  return true;
}
static bool __qi_registrationFile = _qiregisterFile();
QI_REGISTER_IMPLEMENTATION(File, FileImpl);

FilePtr openLocalFile(const qi::Path& localPath)
{
  return boost::make_shared<FileImpl>(localPath);
}
}
