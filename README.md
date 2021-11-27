# softsec

## Requirements

- Visual Studio Community 2019
- Windows 10 SDK, version 2004 (10.0.19041.685)
- Windows 10 WDK, version 2004 (10.0.19041.685)
- Visual Studio Code
- Doxygen

## Roadmap

- SS::Driver
  - [x] `BOOL SS::Driver::Load(LPCTSTR lpServiceName, LPCTSTR lpBinaryPathName);`
  - [x] `BOOL SS::Driver::Unload(LPCTSTR lpServiceName)`
- SS::VirtualMemory
  - [ ] SS::VirtualMemory::Allocate
  - [ ] SS::VirtualMemory::Free
  - [ ] SS::VirtualMemory::Read
  - [ ] SS::VirtualMemory::Write
  - [ ] SS::VirtualMemory::Lock
  - [ ] SS::VirtualMemory::Unlock
  - [ ] SS::VirtualMemory::Protect
  - [ ] SS::VirtualMemory::Query
  - [ ] SS::VirtualMemory::SetInformation
  - [ ] SS::VirtualMemory::Flush
  - [ ] SS::VirtualMemory::AllocateEx
