CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT( xbvm )

FIND_PACKAGE( Threads REQUIRED )
FIND_PACKAGE( SDL REQUIRED )

ADD_DEFINITIONS( -Werror -Wextra -Wall )

INCLUDE_DIRECTORIES( ${xbvm_SOURCE_DIR} )

ADD_EXECUTABLE( xbvm audio/Ac97Codec
                     audio/Apu
                     bridges/AgpBridge
                     bridges/DramController
                     bridges/HostBridge
                     bridges/IsaBridge
                     graphics/BusControlEngine
                     graphics/CrtcEngine
                     graphics/Engine
                     graphics/MasterControlEngine
                     graphics/MemoryInterfaceEngine
                     graphics/Nv2a
                     graphics/RamDacEngine
                     graphics/StrapsEngine
                     graphics/UnknownEngine
                     graphics/VgaEmulationEngine
                     graphics/VgaSequencerEngine
                     graphics/VideoOverlayEngine
                     ide/DvdDrive
                     ide/FlatDiskImage
                     ide/HardDrive
                     ide/IdeController
                     ide/IdeDevice
                     interp/InterpreterCpu
                     interp/InterpreterCpu_sys
                     kvm/KvmCpu
                     net/NetworkAdapter
                     smbus/ConexantEncoder
                     smbus/PIC16L
                     smbus/SerialEeprom
                     smbus/SmBusController
                     smbus/SmBusDevice
                     smbus/TemperatureMonitor
                     superio/Cmos
                     superio/DmaController
                     superio/Pic
                     superio/Pit
                     superio/SuperIoController
                     usb/OhciController
                     Cpu
                     HostDevice
                     Main
                     Mem
                     PciBus
                     PciDevice
                     Sdl
                     Xbox )

TARGET_LINK_LIBRARIES( xbvm ${CMAKE_THREAD_LIBS_INIT} 
                            ${SDL_LIBRARY} )

