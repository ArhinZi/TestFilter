#pragma warning(push)
#pragma warning(disable : 4201 4324)

#if defined(__cplusplus)

extern "C" {

#endif // #if defined(__cplusplus)


#define IRP_MJ_CREATE                   0x00
#define IRP_MJ_CREATE_NAMED_PIPE        0x01
#define IRP_MJ_CLOSE                    0x02
#define IRP_MJ_READ                     0x03
#define IRP_MJ_WRITE                    0x04
#define IRP_MJ_QUERY_INFORMATION        0x05
#define IRP_MJ_SET_INFORMATION          0x06
#define IRP_MJ_QUERY_EA                 0x07
#define IRP_MJ_SET_EA                   0x08
#define IRP_MJ_FLUSH_BUFFERS            0x09
#define IRP_MJ_QUERY_VOLUME_INFORMATION 0x0a
#define IRP_MJ_SET_VOLUME_INFORMATION   0x0b
#define IRP_MJ_DIRECTORY_CONTROL        0x0c
#define IRP_MJ_FILE_SYSTEM_CONTROL      0x0d
#define IRP_MJ_DEVICE_CONTROL           0x0e
#define IRP_MJ_INTERNAL_DEVICE_CONTROL  0x0f
#define IRP_MJ_SHUTDOWN                 0x10
#define IRP_MJ_LOCK_CONTROL             0x11
#define IRP_MJ_CLEANUP                  0x12
#define IRP_MJ_CREATE_MAILSLOT          0x13
#define IRP_MJ_QUERY_SECURITY           0x14
#define IRP_MJ_SET_SECURITY             0x15
#define IRP_MJ_POWER                    0x16
#define IRP_MJ_SYSTEM_CONTROL           0x17
#define IRP_MJ_DEVICE_CHANGE            0x18
#define IRP_MJ_QUERY_QUOTA              0x19
#define IRP_MJ_SET_QUOTA                0x1a
#define IRP_MJ_PNP                      0x1b
#define IRP_MJ_PNP_POWER                IRP_MJ_PNP      // Obsolete....
#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

//
// PNP minor function codes.
//

#define IRP_MN_START_DEVICE                 0x00
#define IRP_MN_QUERY_REMOVE_DEVICE          0x01
#define IRP_MN_REMOVE_DEVICE                0x02
#define IRP_MN_CANCEL_REMOVE_DEVICE         0x03
#define IRP_MN_STOP_DEVICE                  0x04
#define IRP_MN_QUERY_STOP_DEVICE            0x05
#define IRP_MN_CANCEL_STOP_DEVICE           0x06

#define IRP_MN_QUERY_DEVICE_RELATIONS       0x07
#define IRP_MN_QUERY_INTERFACE              0x08
#define IRP_MN_QUERY_CAPABILITIES           0x09
#define IRP_MN_QUERY_RESOURCES              0x0A
#define IRP_MN_QUERY_RESOURCE_REQUIREMENTS  0x0B
#define IRP_MN_QUERY_DEVICE_TEXT            0x0C
#define IRP_MN_FILTER_RESOURCE_REQUIREMENTS 0x0D

#define IRP_MN_READ_CONFIG                  0x0F
#define IRP_MN_WRITE_CONFIG                 0x10
#define IRP_MN_EJECT                        0x11
#define IRP_MN_SET_LOCK                     0x12
#define IRP_MN_QUERY_ID                     0x13
#define IRP_MN_QUERY_PNP_DEVICE_STATE       0x14
#define IRP_MN_QUERY_BUS_INFORMATION        0x15
#define IRP_MN_DEVICE_USAGE_NOTIFICATION    0x16
#define IRP_MN_SURPRISE_REMOVAL             0x17


//
// POWER minor function codes
//
#define IRP_MN_WAIT_WAKE                    0x00
#define IRP_MN_POWER_SEQUENCE               0x01
#define IRP_MN_SET_POWER                    0x02
#define IRP_MN_QUERY_POWER                  0x03

#if ARCHITECTURE == MACRO_ARCH_X86
#if CST_OSVER == MACRO_OSVER_XP


// ***** Kernel data types for XP x86 *****
// ----------------------------------------

#pragma pack(push, 8)

typedef short CSHORT;
typedef UCHAR KIRQL;
typedef CCHAR KPROCESSOR_MODE;
typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

typedef enum {
    BusQueryDeviceID = 0,       
    BusQueryHardwareIDs = 1,    
    BusQueryCompatibleIDs = 2,  
    BusQueryInstanceID = 3,     
    BusQueryDeviceSerialNumber = 4    
} BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;



typedef enum _DEVICE_RELATION_TYPE {
    BusRelations,
    EjectionRelations,
    PowerRelations,
    RemovalRelations,
    TargetDeviceRelation,
    SingleBusRelations
} DEVICE_RELATION_TYPE, *PDEVICE_RELATION_TYPE;

typedef enum {
    DeviceTextDescription = 0,            
    DeviceTextLocationInformation = 1     
} DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;

typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE {
    DeviceUsageTypeUndefined,
    DeviceUsageTypePaging,
    DeviceUsageTypeHibernation,
    DeviceUsageTypeDumpFile
} DEVICE_USAGE_NOTIFICATION_TYPE;



typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   
    FileBothDirectoryInformation,   
    FileBasicInformation,           
    FileStandardInformation,        
    FileInternalInformation,        
    FileEaInformation,              
    FileAccessInformation,          
    FileNameInformation,            
    FileRenameInformation,          
    FileLinkInformation,            
    FileNamesInformation,           
    FileDispositionInformation,     
    FilePositionInformation,        
    FileFullEaInformation,          
    FileModeInformation,            
    FileAlignmentInformation,       
    FileAllInformation,             
    FileAllocationInformation,      
    FileEndOfFileInformation,       
    FileAlternateNameInformation,   
    FileStreamInformation,          
    FilePipeInformation,            
    FilePipeLocalInformation,       
    FilePipeRemoteInformation,      
    FileMailslotQueryInformation,   
    FileMailslotSetInformation,     
    FileCompressionInformation,     
    FileObjectIdInformation,        
    FileCompletionInformation,      
    FileMoveClusterInformation,     
    FileQuotaInformation,           
    FileReparsePointInformation,    
    FileNetworkOpenInformation,     
    FileAttributeTagInformation,    
    FileTrackingInformation,        
    FileIdBothDirectoryInformation, 
    FileIdFullDirectoryInformation, 
    FileValidDataLengthInformation, 
    FileShortNameInformation,       
    FileIoCompletionNotificationInformation, 
    FileIoStatusBlockRangeInformation,       
    FileIoPriorityHintInformation,           
    FileSfioReserveInformation,              
    FileSfioVolumeInformation,               
    FileHardLinkInformation,                 
    FileProcessIdsUsingFileInformation,      
    FileNormalizedNameInformation,           
    FileNetworkPhysicalNameInformation,      
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef enum _FSINFOCLASS {
    FileFsVolumeInformation       = 1,
    FileFsLabelInformation,      
    FileFsSizeInformation,       
    FileFsDeviceInformation,     
    FileFsAttributeInformation,  
    FileFsControlInformation,    
    FileFsFullSizeInformation,   
    FileFsObjectIdInformation,   
    FileFsDriverPathInformation, 
    FileFsVolumeFlagsInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;


typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;


typedef enum _IO_ALLOCATION_ACTION {
    KeepObject = 1,
    DeallocateObject,
    DeallocateObjectKeepRegisters
} IO_ALLOCATION_ACTION, *PIO_ALLOCATION_ACTION;

typedef enum _IRQ_DEVICE_POLICY {
    IrqPolicyMachineDefault = 0,
    IrqPolicyAllCloseProcessors,
    IrqPolicyOneCloseProcessor,
    IrqPolicyAllProcessorsInMachine,
    IrqPolicySpecifiedProcessors,
    IrqPolicySpreadMessagesAcrossAllProcessors
} IRQ_DEVICE_POLICY, *PIRQ_DEVICE_POLICY;

typedef enum _IRQ_PRIORITY {
    IrqPriorityUndefined = 0,
    IrqPriorityLow,
    IrqPriorityNormal,
    IrqPriorityHigh
} IRQ_PRIORITY, *PIRQ_PRIORITY;



typedef enum _POWER_STATE_TYPE {
    SystemPowerState = 0,
    DevicePowerState
} POWER_STATE_TYPE, *PPOWER_STATE_TYPE;


typedef
IO_ALLOCATION_ACTION
(*PDRIVER_CONTROL) (
     struct _DEVICE_OBJECT *DeviceObject,
     struct _IRP *Irp,
     PVOID MapRegisterBase,
     PVOID Context
    );


typedef struct _KDEVICE_QUEUE_ENTRY {
    LIST_ENTRY DeviceListEntry;
    ULONG SortKey;
    BOOLEAN Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY, *PRKDEVICE_QUEUE_ENTRY;

typedef
void
(*PKDEFERRED_ROUTINE) (
     struct _KDPC *Dpc,
     PVOID DeferredContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );



typedef struct _KDPC {
    UCHAR Type;
    UCHAR Importance;
    USHORT Number;
    LIST_ENTRY DpcListEntry;
    PKDEFERRED_ROUTINE DeferredRoutine;
    PVOID DeferredContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
     PVOID DpcData;
} KDPC, *PKDPC, *PRKDPC;



typedef struct _WAIT_CONTEXT_BLOCK {
    KDEVICE_QUEUE_ENTRY WaitQueueEntry;
    PDRIVER_CONTROL DeviceRoutine;
    PVOID DeviceContext;
    ULONG NumberOfMapRegisters;
    PVOID DeviceObject;
    PVOID CurrentIrp;
    PKDPC BufferChainingDpc;
} WAIT_CONTEXT_BLOCK, *PWAIT_CONTEXT_BLOCK;


typedef struct _VPB *PVPB;


typedef struct _IO_TIMER *PIO_TIMER;

typedef struct _KDEVICE_QUEUE {
    CSHORT Type;
    CSHORT Size;
    LIST_ENTRY DeviceListHead;
    KSPIN_LOCK Lock;












    BOOLEAN Busy;


} KDEVICE_QUEUE, *PKDEVICE_QUEUE, *PRKDEVICE_QUEUE;

typedef struct _DISPATCHER_HEADER {
    union {
        struct {
            UCHAR Type;
            union {
                UCHAR Abandoned;
                UCHAR Absolute;
                UCHAR NpxIrql;
                BOOLEAN Signalling;
            };

            union {
                UCHAR Size;
                UCHAR Hand;
            };

            union {
                UCHAR Inserted;
                BOOLEAN DebugActive;
                BOOLEAN DpcActive;
            };
        };

        volatile LONG Lock;
    };

    LONG SignalState;
    LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;



typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;


typedef struct __declspec(align(8)) _DEVICE_OBJECT {
    CSHORT Type;
    USHORT Size;
    LONG ReferenceCount;
    struct _DRIVER_OBJECT *DriverObject;
    struct _DEVICE_OBJECT *NextDevice;
    struct _DEVICE_OBJECT *AttachedDevice;
    struct _IRP *CurrentIrp;
    PIO_TIMER Timer;
    ULONG Flags;                                
    ULONG Characteristics;                      
     PVPB Vpb;
    PVOID DeviceExtension;
    ULONG DeviceType;
    CCHAR StackSize;
    union {
        LIST_ENTRY ListEntry;
        WAIT_CONTEXT_BLOCK Wcb;
    } Queue;
    ULONG AlignmentRequirement;
    KDEVICE_QUEUE DeviceQueue;
    KDPC Dpc;

    
    
    
    

    ULONG ActiveThreadCount;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    KEVENT DeviceLock;

    USHORT SectorSize;
    USHORT Spare1;

    struct _DEVOBJ_EXTENSION  *DeviceObjectExtension;
    PVOID  Reserved;
} DEVICE_OBJECT;

typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT; 

typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS;
typedef SECTION_OBJECT_POINTERS *PSECTION_OBJECT_POINTERS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;



     PWCH   Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;

typedef struct _IO_COMPLETION_CONTEXT {
    PVOID Port;
    PVOID Key;
} IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;

typedef struct _FILE_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    PVPB Vpb;
    PVOID FsContext;
    PVOID FsContext2;
    PSECTION_OBJECT_POINTERS SectionObjectPointer;
    PVOID PrivateCacheMap;
    NTSTATUS FinalStatus;
    struct _FILE_OBJECT *RelatedFileObject;
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    ULONG Flags;
    UNICODE_STRING FileName;
    LARGE_INTEGER CurrentByteOffset;
     ULONG Waiters;
     ULONG Busy;
    PVOID LastLock;
    KEVENT Lock;
    KEVENT Event;
     PIO_COMPLETION_CONTEXT CompletionContext;
    KSPIN_LOCK IrpListLock;
    LIST_ENTRY IrpList;
     PVOID FileObjectExtension;
} FILE_OBJECT;
typedef struct _FILE_OBJECT *PFILE_OBJECT; 

typedef struct _ETHREAD *PETHREAD;


typedef
void
DRIVER_CANCEL (
     struct _DEVICE_OBJECT *DeviceObject,
     struct _IRP *Irp
    );

typedef DRIVER_CANCEL *PDRIVER_CANCEL;


typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;


typedef
void
(__stdcall *PIO_APC_ROUTINE) (
     PVOID ApcContext,
     PIO_STATUS_BLOCK IoStatusBlock,
     ULONG Reserved
    );



typedef  struct _MDL {
    struct _MDL *Next;
    CSHORT Size;
    CSHORT MdlFlags;
    struct _EPROCESS *Process;
    PVOID MappedSystemVa;
    PVOID StartVa;
    ULONG ByteCount;
    ULONG ByteOffset;
} MDL, *PMDL;

struct _KAPC;
typedef
void
(*PKNORMAL_ROUTINE) (
     PVOID NormalContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );

typedef
void
(*PKKERNEL_ROUTINE) (
     struct _KAPC *Apc,
      PKNORMAL_ROUTINE *NormalRoutine,
      PVOID *NormalContext,
      PVOID *SystemArgument1,
      PVOID *SystemArgument2
    );

typedef
void
(*PKRUNDOWN_ROUTINE) (
     struct _KAPC *Apc
    );

typedef
BOOLEAN
(*PKSYNCHRONIZE_ROUTINE) (
     PVOID SynchronizeContext
    );

typedef
BOOLEAN
(*PKTRANSFER_ROUTINE) (
    void
    );




typedef struct _KAPC {
    UCHAR Type;
    UCHAR SpareByte0;
    UCHAR Size;
    UCHAR SpareByte1;
    ULONG SpareLong0;
    struct _KTHREAD *Thread;
    LIST_ENTRY ApcListEntry;
    PKKERNEL_ROUTINE KernelRoutine;
    PKRUNDOWN_ROUTINE RundownRoutine;
    PKNORMAL_ROUTINE NormalRoutine;
    PVOID NormalContext;

    
    
    

    PVOID SystemArgument1;
    PVOID SystemArgument2;
    CCHAR ApcStateIndex;
    KPROCESSOR_MODE ApcMode;
    BOOLEAN Inserted;
} KAPC, *PKAPC, *PRKAPC;


typedef struct __declspec(align(8)) _IRP {
    CSHORT Type;
    USHORT Size;

    
    
    

    
    
    
    

    PMDL MdlAddress;

    
    
    

    ULONG Flags;

    
    
    
    
    
    
    
    
    
    
    
    
    

    union  {
        struct _IRP *MasterIrp;
         LONG IrpCount;
        PVOID SystemBuffer;
    } AssociatedIrp;

    
    
    
    

    LIST_ENTRY ThreadListEntry;

    
    
    

    IO_STATUS_BLOCK IoStatus;

    
    
    

    KPROCESSOR_MODE RequestorMode;

    
    
    
    

    BOOLEAN PendingReturned;

    
    
    

    CHAR StackCount;
    CHAR CurrentLocation;

    
    
    

    BOOLEAN Cancel;

    
    
    

    KIRQL CancelIrql;

    
    
    
    

    CCHAR ApcEnvironment;

    
    
    

    UCHAR AllocationFlags;

    
    
    

    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;
    union {
        struct {
            union {
                PIO_APC_ROUTINE UserApcRoutine;
                PVOID IssuingProcess;
            };
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    
    
    
    

     PDRIVER_CANCEL CancelRoutine;

    
    
    
    
    
    
    

    PVOID UserBuffer;

    
    
    
    
    
    
    
    
    

    union {

        struct {

            union {

                
                
                
                

                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;

                struct {

                    
                    
                    
                    
                    

                    PVOID DriverContext[4];

                } ;

            } ;

            
            
            

            PETHREAD Thread;

            
            
            
            
            

            PCHAR AuxiliaryBuffer;

            
            
            
            
            

            struct {

                
                
                
                

                LIST_ENTRY ListEntry;

                union {

                    
                    
                    
                    
                    
                    

                    struct _IO_STACK_LOCATION *CurrentStackLocation;

                    
                    
                    

                    ULONG PacketType;
                };
            };

            
            
            
            
            

            PFILE_OBJECT OriginalFileObject;

        } Overlay;

        
        
        
        
        
        
        

        KAPC Apc;

        
        
        
        

        PVOID CompletionKey;

    } Tail;

} IRP, *PIRP;


typedef struct _ACCESS_STATE *PACCESS_STATE;


typedef struct _IO_SECURITY_CONTEXT {
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_STATE AccessState;
    ACCESS_MASK DesiredAccess;
    ULONG FullCreateOptions;
} IO_SECURITY_CONTEXT, *PIO_SECURITY_CONTEXT;

typedef struct _FILE_GET_QUOTA_INFORMATION *PFILE_GET_QUOTA_INFORMATION;

typedef void (*PINTERFACE_REFERENCE)(PVOID Context);
typedef void (*PINTERFACE_DEREFERENCE)(PVOID Context);


typedef struct _INTERFACE {
    USHORT Size;
    USHORT Version;
    PVOID Context;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;
    
} CST_INTERFACE, *PINTERFACE;

typedef  struct _DEVICE_CAPABILITIES {
    USHORT Size;
    USHORT Version;  
    ULONG DeviceD1:1;
    ULONG DeviceD2:1;
    ULONG LockSupported:1;
    ULONG EjectSupported:1; 
    ULONG Removable:1;
    ULONG DockDevice:1;
    ULONG UniqueID:1;
    ULONG SilentInstall:1;
    ULONG RawDeviceOK:1;
    ULONG SurpriseRemovalOK:1;
    ULONG WakeFromD0:1;
    ULONG WakeFromD1:1;
    ULONG WakeFromD2:1;
    ULONG WakeFromD3:1;
    ULONG HardwareDisabled:1;
    ULONG NonDynamic:1;
    ULONG WarmEjectSupported:1;
    ULONG NoDisplayInUI:1;
    ULONG Reserved:14;

    ULONG Address;
    ULONG UINumber;

    DEVICE_POWER_STATE DeviceState[7];
    SYSTEM_POWER_STATE SystemWake;
    DEVICE_POWER_STATE DeviceWake;
    ULONG D1Latency;
    ULONG D2Latency;
    ULONG D3Latency;
} DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;


typedef struct _IO_RESOURCE_DESCRIPTOR {
    UCHAR Option;
    UCHAR Type;                         
    UCHAR ShareDisposition;             
    UCHAR Spare1;
    USHORT Flags;                       
    USHORT Spare2;                      

    union {
        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Port;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory;

        struct {
            ULONG MinimumVector;
            ULONG MaximumVector;
            IRQ_DEVICE_POLICY AffinityPolicy;
            IRQ_PRIORITY PriorityPolicy;
            KAFFINITY TargetedProcessors;
        } Interrupt;

        struct {
            ULONG MinimumChannel;
            ULONG MaximumChannel;
        } Dma;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Generic;

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Length;
            ULONG MinBusNumber;
            ULONG MaxBusNumber;
            ULONG Reserved;
        } BusNumber;

        struct {
            ULONG Priority;   
            ULONG Reserved1;
            ULONG Reserved2;
        } ConfigData;

        
        
        
        

        struct {
            ULONG Length40;
            ULONG Alignment40;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory40;

        struct {
            ULONG Length48;
            ULONG Alignment48;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory48;

        struct {
            ULONG Length64;
            ULONG Alignment64;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory64;


    } u;

} IO_RESOURCE_DESCRIPTOR, *PIO_RESOURCE_DESCRIPTOR;




typedef struct _IO_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;

    ULONG Count;
    IO_RESOURCE_DESCRIPTOR Descriptors[1];
} IO_RESOURCE_LIST, *PIO_RESOURCE_LIST;



typedef struct _IO_RESOURCE_REQUIREMENTS_LIST {
    ULONG ListSize;
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    ULONG SlotNumber;
    ULONG Reserved[3];
    ULONG AlternativeLists;
    IO_RESOURCE_LIST  List[1];
} IO_RESOURCE_REQUIREMENTS_LIST, *PIO_RESOURCE_REQUIREMENTS_LIST;

typedef struct _POWER_SEQUENCE {
    ULONG SequenceD1;
    ULONG SequenceD2;
    ULONG SequenceD3;
} POWER_SEQUENCE, *PPOWER_SEQUENCE;

typedef union _POWER_STATE {
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;

#pragma pack(push,4)


typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Generic;

        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        
        

        struct {
            ULONG Level;
            ULONG Vector;
            KAFFINITY Affinity;
        } Interrupt;

        
        
        
        

        struct {
            union {
               struct {
                   USHORT Reserved;
                   USHORT MessageCount;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Raw;

               struct {
                   ULONG Level;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Translated;
            };
        } MessageInterrupt;
        
        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;    
            ULONG Length;
        } Memory;

        
        
        

        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        
        
        
        

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Start;
            ULONG Length;
            ULONG Reserved;
        } BusNumber;
        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;

        
        
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length40;
        } Memory40;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length48;
        } Memory48;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length64;
        } Memory64;
        
        
    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;

#pragma pack(pop)

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;

typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;


typedef struct _CM_RESOURCE_LIST {
    ULONG Count;
    CM_FULL_RESOURCE_DESCRIPTOR List[1];
} CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;


typedef
NTSTATUS
IO_COMPLETION_ROUTINE (
     PDEVICE_OBJECT DeviceObject,
     PIRP Irp,
     PVOID Context
    );

typedef IO_COMPLETION_ROUTINE *PIO_COMPLETION_ROUTINE;

#pragma pack(push,4)


typedef struct _IO_STACK_LOCATION {
    UCHAR MajorFunction;
    UCHAR MinorFunction;
    UCHAR Flags;
    UCHAR Control;

    
    
    
    
    

    union {

        
        
        

        struct {
            PIO_SECURITY_CONTEXT SecurityContext;
            ULONG Options;
            USHORT  FileAttributes;
            USHORT ShareAccess;
            ULONG  EaLength;
        } Create;


        
        
        

        struct {
            ULONG Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } Read;

        
        
        

        struct {
            ULONG Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } Write;

        
        
        

        struct {
            ULONG Length;
            PUNICODE_STRING FileName;
            FILE_INFORMATION_CLASS FileInformationClass;
            ULONG  FileIndex;
        } QueryDirectory;

        
        
        

        struct {
            ULONG Length;
            ULONG  CompletionFilter;
        } NotifyDirectory;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS  FileInformationClass;
        } QueryFile;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS  FileInformationClass;
            PFILE_OBJECT FileObject;
            union {
                struct {
                    BOOLEAN ReplaceIfExists;
                    BOOLEAN AdvanceOnly;
                };
                ULONG ClusterCount;
                HANDLE DeleteHandle;
            };
        } SetFile;



        
        
        

        struct {
            ULONG Length;
            PVOID EaList;
            ULONG EaListLength;
            ULONG  EaIndex;
        } QueryEa;

        
        
        

        struct {
            ULONG Length;
        } SetEa;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS  FsInformationClass;
        } QueryVolume;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS  FsInformationClass;
        } SetVolume;

        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG  InputBufferLength;
            ULONG  FsControlCode;
            PVOID Type3InputBuffer;
        } FileSystemControl;
        
        
        

        struct {
            PLARGE_INTEGER Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } LockControl;

        
        
        
        
        



        
        
        
        
        



        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG  InputBufferLength;
            ULONG  IoControlCode;
            PVOID Type3InputBuffer;
        } DeviceIoControl;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            ULONG  Length;
        } QuerySecurity;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            PSECURITY_DESCRIPTOR SecurityDescriptor;
        } SetSecurity;

        
        
        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } MountVolume;

        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } VerifyVolume;

        
        
        

        struct {
            struct _SCSI_REQUEST_BLOCK *Srb;
        } Scsi;



        
        
        

        struct {
            ULONG Length;
            PSID StartSid;
            PFILE_GET_QUOTA_INFORMATION SidList;
            ULONG SidListLength;
        } QueryQuota;

        
        
        

        struct {
            ULONG Length;
        } SetQuota;



        
        
        

        struct {
            DEVICE_RELATION_TYPE Type;
        } QueryDeviceRelations;

        
        
        

        struct {
            const GUID *InterfaceType;
            USHORT Size;
            USHORT Version;
            PINTERFACE Interface;
            PVOID InterfaceSpecificData;
        } QueryInterface;

        
        
        

        struct {
            PDEVICE_CAPABILITIES Capabilities;
        } DeviceCapabilities;

        
        
        

        struct {
            PIO_RESOURCE_REQUIREMENTS_LIST IoResourceRequirementList;
        } FilterResourceRequirements;

        
        
        

        struct {
            ULONG WhichSpace;
            PVOID Buffer;
            ULONG Offset;
            ULONG  Length;
        } ReadWriteConfig;

        
        
        

        struct {
            BOOLEAN Lock;
        } SetLock;

        
        
        

        struct {
            BUS_QUERY_ID_TYPE IdType;
        } QueryId;

        
        
        

        struct {
            DEVICE_TEXT_TYPE DeviceTextType;
            LCID  LocaleId;
        } QueryDeviceText;

        
        
        

        struct {
            BOOLEAN InPath;
            BOOLEAN Reserved[3];
            DEVICE_USAGE_NOTIFICATION_TYPE  Type;
        } UsageNotification;

        
        
        

        struct {
            SYSTEM_POWER_STATE PowerState;
        } WaitWake;

        
        
        

        struct {
            PPOWER_SEQUENCE PowerSequence;
        } PowerSequence;

        
        
        











        struct {
            ULONG SystemContext;
            POWER_STATE_TYPE  Type;
            POWER_STATE  State;
            POWER_ACTION  ShutdownType;
        } Power;

        
        
        

        struct {
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
        } StartDevice;

        
        
        
        
        

        
        
        

        struct {
            ULONG_PTR ProviderId;
            PVOID DataPath;
            ULONG BufferSize;
            PVOID Buffer;
        } WMI;

        
        
        

        struct {
            PVOID Argument1;
            PVOID Argument2;
            PVOID Argument3;
            PVOID Argument4;
        } Others;

    } Parameters;

    
    
    
    

    PDEVICE_OBJECT DeviceObject;

    
    
    
    

    PFILE_OBJECT FileObject;

    
    
    
    

    PIO_COMPLETION_ROUTINE CompletionRoutine;

    
    
    
    

    PVOID Context;

} IO_STACK_LOCATION, *PIO_STACK_LOCATION;

#pragma pack(pop)
#pragma pack(pop)

#elif CST_OSVER == MACRO_OSVER_LH // #if CST_OSVER == MACRO_OSVER_XP

// ***** Kernel data types for Vista x86 *****
// -------------------------------------------

#pragma pack(push, 8)

typedef short CSHORT;
typedef char CCHAR;          
typedef unsigned char UCHAR;
typedef CCHAR KPROCESSOR_MODE;
typedef UCHAR KIRQL;

typedef enum {
    BusQueryDeviceID = 0,       
    BusQueryHardwareIDs = 1,    
    BusQueryCompatibleIDs = 2,  
    BusQueryInstanceID = 3,     
    BusQueryDeviceSerialNumber = 4    
} BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;

typedef enum _DEVICE_RELATION_TYPE {
    BusRelations,
    EjectionRelations,
    PowerRelations,
    RemovalRelations,
    TargetDeviceRelation,
    SingleBusRelations
} DEVICE_RELATION_TYPE, *PDEVICE_RELATION_TYPE;

typedef enum {
    DeviceTextDescription = 0,            
    DeviceTextLocationInformation = 1     
} DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;

typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE {
    DeviceUsageTypeUndefined,
    DeviceUsageTypePaging,
    DeviceUsageTypeHibernation,
    DeviceUsageTypeDumpFile
} DEVICE_USAGE_NOTIFICATION_TYPE;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   
    FileBothDirectoryInformation,   
    FileBasicInformation,           
    FileStandardInformation,        
    FileInternalInformation,        
    FileEaInformation,              
    FileAccessInformation,          
    FileNameInformation,            
    FileRenameInformation,          
    FileLinkInformation,            
    FileNamesInformation,           
    FileDispositionInformation,     
    FilePositionInformation,        
    FileFullEaInformation,          
    FileModeInformation,            
    FileAlignmentInformation,       
    FileAllInformation,             
    FileAllocationInformation,      
    FileEndOfFileInformation,       
    FileAlternateNameInformation,   
    FileStreamInformation,          
    FilePipeInformation,            
    FilePipeLocalInformation,       
    FilePipeRemoteInformation,      
    FileMailslotQueryInformation,   
    FileMailslotSetInformation,     
    FileCompressionInformation,     
    FileObjectIdInformation,        
    FileCompletionInformation,      
    FileMoveClusterInformation,     
    FileQuotaInformation,           
    FileReparsePointInformation,    
    FileNetworkOpenInformation,     
    FileAttributeTagInformation,    
    FileTrackingInformation,        
    FileIdBothDirectoryInformation, 
    FileIdFullDirectoryInformation, 
    FileValidDataLengthInformation, 
    FileShortNameInformation,       
    FileIoCompletionNotificationInformation, 
    FileIoStatusBlockRangeInformation,       
    FileIoPriorityHintInformation,           
    FileSfioReserveInformation,              
    FileSfioVolumeInformation,               
    FileHardLinkInformation,                 
    FileProcessIdsUsingFileInformation,      
    FileNormalizedNameInformation,           
    FileNetworkPhysicalNameInformation,      
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef enum _FSINFOCLASS {
    FileFsVolumeInformation       = 1,
    FileFsLabelInformation,      
    FileFsSizeInformation,       
    FileFsDeviceInformation,     
    FileFsAttributeInformation,  
    FileFsControlInformation,    
    FileFsFullSizeInformation,   
    FileFsObjectIdInformation,   
    FileFsDriverPathInformation, 
    FileFsVolumeFlagsInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef enum _IRQ_DEVICE_POLICY {
    IrqPolicyMachineDefault = 0,
    IrqPolicyAllCloseProcessors,
    IrqPolicyOneCloseProcessor,
    IrqPolicyAllProcessorsInMachine,
    IrqPolicySpecifiedProcessors,
    IrqPolicySpreadMessagesAcrossAllProcessors
} IRQ_DEVICE_POLICY, *PIRQ_DEVICE_POLICY;

typedef enum _IRQ_PRIORITY {
    IrqPriorityUndefined = 0,
    IrqPriorityLow,
    IrqPriorityNormal,
    IrqPriorityHigh
} IRQ_PRIORITY, *PIRQ_PRIORITY;


typedef enum _POWER_STATE_TYPE {
    SystemPowerState = 0,
    DevicePowerState
} POWER_STATE_TYPE, *PPOWER_STATE_TYPE;


typedef  struct _MDL {
    struct _MDL *Next;
    CSHORT Size;
    CSHORT MdlFlags;
    struct _EPROCESS *Process;
    PVOID MappedSystemVa;
    PVOID StartVa;
    ULONG ByteCount;
    ULONG ByteOffset;
} MDL, *PMDL;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _DISPATCHER_HEADER {
    union {
        struct {
            UCHAR Type;
            union {
                UCHAR Abandoned;
                UCHAR Absolute;
                UCHAR NpxIrql;
                BOOLEAN Signalling;
            };

            union {
                UCHAR Size;
                UCHAR Hand;
            };

            union {
                UCHAR Inserted;
                BOOLEAN DebugActive;
                BOOLEAN DpcActive;
            };
        };

        volatile LONG Lock;
    };

    LONG SignalState;
    LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;

typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;

typedef
void
(__stdcall *PIO_APC_ROUTINE) (
     PVOID ApcContext,
     PIO_STATUS_BLOCK IoStatusBlock,
     ULONG Reserved
    );

typedef
void
DRIVER_CANCEL (
     struct _DEVICE_OBJECT *DeviceObject,
     struct _IRP *Irp
    );

typedef DRIVER_CANCEL *PDRIVER_CANCEL;


typedef struct _KDEVICE_QUEUE_ENTRY {
    LIST_ENTRY DeviceListEntry;
    ULONG SortKey;
    BOOLEAN Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY, *PRKDEVICE_QUEUE_ENTRY;

typedef struct _ETHREAD *PETHREAD;

typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT; 

typedef struct _VPB *PVPB;

typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS;
typedef SECTION_OBJECT_POINTERS *PSECTION_OBJECT_POINTERS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;



     PWCH   Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _IO_COMPLETION_CONTEXT {
    PVOID Port;
    PVOID Key;
} IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;


typedef struct _FILE_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    PVPB Vpb;
    PVOID FsContext;
    PVOID FsContext2;
    PSECTION_OBJECT_POINTERS SectionObjectPointer;
    PVOID PrivateCacheMap;
    NTSTATUS FinalStatus;
    struct _FILE_OBJECT *RelatedFileObject;
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    ULONG Flags;
    UNICODE_STRING FileName;
    LARGE_INTEGER CurrentByteOffset;
     ULONG Waiters;
     ULONG Busy;
    PVOID LastLock;
    KEVENT Lock;
    KEVENT Event;
     PIO_COMPLETION_CONTEXT CompletionContext;
    KSPIN_LOCK IrpListLock;
    LIST_ENTRY IrpList;
     PVOID FileObjectExtension;
} FILE_OBJECT;
typedef struct _FILE_OBJECT *PFILE_OBJECT; 



struct _KAPC;

typedef
void
(*PKNORMAL_ROUTINE) (
     PVOID NormalContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );

typedef
void
(*PKKERNEL_ROUTINE) (
     struct _KAPC *Apc,
      PKNORMAL_ROUTINE *NormalRoutine,
      PVOID *NormalContext,
      PVOID *SystemArgument1,
      PVOID *SystemArgument2
    );

typedef
void
(*PKRUNDOWN_ROUTINE) (
     struct _KAPC *Apc
    );

typedef
BOOLEAN
(*PKSYNCHRONIZE_ROUTINE) (
     PVOID SynchronizeContext
    );

typedef
BOOLEAN
(*PKTRANSFER_ROUTINE) (
    void
    );


typedef struct _KAPC {
    UCHAR Type;
    UCHAR SpareByte0;
    UCHAR Size;
    UCHAR SpareByte1;
    ULONG SpareLong0;
    struct _KTHREAD *Thread;
    LIST_ENTRY ApcListEntry;
    PKKERNEL_ROUTINE KernelRoutine;
    PKRUNDOWN_ROUTINE RundownRoutine;
    PKNORMAL_ROUTINE NormalRoutine;
    PVOID NormalContext;

    
    
    

    PVOID SystemArgument1;
    PVOID SystemArgument2;
    CCHAR ApcStateIndex;
    KPROCESSOR_MODE ApcMode;
    BOOLEAN Inserted;
} KAPC, *PKAPC, *PRKAPC;

typedef struct __declspec(align(8)) _IRP {
    CSHORT Type;
    USHORT Size;

    
    
    

    
    
    
    

    PMDL MdlAddress;

    
    
    

    ULONG Flags;

    
    
    
    
    
    
    
    
    
    
    
    
    

    union {
        struct _IRP *MasterIrp;
         LONG IrpCount;
        PVOID SystemBuffer;
    } AssociatedIrp;

    
    
    
    

    LIST_ENTRY ThreadListEntry;

    
    
    

    IO_STATUS_BLOCK IoStatus;

    
    
    

    KPROCESSOR_MODE RequestorMode;

    
    
    
    

    BOOLEAN PendingReturned;

    
    
    

    CHAR StackCount;
    CHAR CurrentLocation;

    
    
    

    BOOLEAN Cancel;

    
    
    

    KIRQL CancelIrql;

    
    
    
    

    CCHAR ApcEnvironment;

    
    
    

    UCHAR AllocationFlags;

    
    
    

    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;
    union {
        struct {
            union {
                PIO_APC_ROUTINE UserApcRoutine;
                PVOID IssuingProcess;
            };
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    
    
    
    

     PDRIVER_CANCEL CancelRoutine;

    
    
    
    
    
    
    

    PVOID UserBuffer;

    
    
    
    
    
    
    
    
    

    union {

        struct {

            union {

                
                
                
                

                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;

                struct {

                    
                    
                    
                    
                    

                    PVOID DriverContext[4];

                } ;

            } ;

            
            
            

            PETHREAD Thread;

            
            
            
            
            

            PCHAR AuxiliaryBuffer;

            
            
            
            
            

            struct {

                
                
                
                

                LIST_ENTRY ListEntry;

                union {

                    
                    
                    
                    
                    
                    

                    struct _IO_STACK_LOCATION *CurrentStackLocation;

                    
                    
                    

                    ULONG PacketType;
                };
            };

            
            
            
            
            

            PFILE_OBJECT OriginalFileObject;

        } Overlay;

        
        
        
        
        
        
        

        KAPC Apc;

        
        
        
        

        PVOID CompletionKey;

    } Tail;

} IRP, *PIRP;




typedef struct _IO_STACK_LOCATION *PIO_STACK_LOCATION;

typedef struct _ACCESS_STATE *PACCESS_STATE;


typedef struct _IO_SECURITY_CONTEXT {
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_STATE AccessState;
    ACCESS_MASK DesiredAccess;
    ULONG FullCreateOptions;
} IO_SECURITY_CONTEXT, *PIO_SECURITY_CONTEXT;



typedef struct _FILE_GET_QUOTA_INFORMATION *PFILE_GET_QUOTA_INFORMATION;

typedef void (*PINTERFACE_REFERENCE)(PVOID Context);
typedef void (*PINTERFACE_DEREFERENCE)(PVOID Context);


typedef struct _INTERFACE {
    USHORT Size;
    USHORT Version;
    PVOID Context;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;
    
} CST_INTERFACE, *PINTERFACE;

typedef  struct _DEVICE_CAPABILITIES {
    USHORT Size;
    USHORT Version;  
    ULONG DeviceD1:1;
    ULONG DeviceD2:1;
    ULONG LockSupported:1;
    ULONG EjectSupported:1; 
    ULONG Removable:1;
    ULONG DockDevice:1;
    ULONG UniqueID:1;
    ULONG SilentInstall:1;
    ULONG RawDeviceOK:1;
    ULONG SurpriseRemovalOK:1;
    ULONG WakeFromD0:1;
    ULONG WakeFromD1:1;
    ULONG WakeFromD2:1;
    ULONG WakeFromD3:1;
    ULONG HardwareDisabled:1;
    ULONG NonDynamic:1;
    ULONG WarmEjectSupported:1;
    ULONG NoDisplayInUI:1;
    ULONG Reserved:14;

    ULONG Address;
    ULONG UINumber;

    DEVICE_POWER_STATE DeviceState[7];
    SYSTEM_POWER_STATE SystemWake;
    DEVICE_POWER_STATE DeviceWake;
    ULONG D1Latency;
    ULONG D2Latency;
    ULONG D3Latency;
} DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;


typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

typedef struct _IO_RESOURCE_DESCRIPTOR {
    UCHAR Option;
    UCHAR Type;                         
    UCHAR ShareDisposition;             
    UCHAR Spare1;
    USHORT Flags;                       
    USHORT Spare2;                      

    union {
        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Port;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory;

        struct {
            ULONG MinimumVector;
            ULONG MaximumVector;
            IRQ_DEVICE_POLICY AffinityPolicy;
            IRQ_PRIORITY PriorityPolicy;
            KAFFINITY TargetedProcessors;
        } Interrupt;

        struct {
            ULONG MinimumChannel;
            ULONG MaximumChannel;
        } Dma;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Generic;

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Length;
            ULONG MinBusNumber;
            ULONG MaxBusNumber;
            ULONG Reserved;
        } BusNumber;

        struct {
            ULONG Priority;   
            ULONG Reserved1;
            ULONG Reserved2;
        } ConfigData;

        
        
        
        

        struct {
            ULONG Length40;
            ULONG Alignment40;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory40;

        struct {
            ULONG Length48;
            ULONG Alignment48;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory48;

        struct {
            ULONG Length64;
            ULONG Alignment64;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory64;


    } u;

} IO_RESOURCE_DESCRIPTOR, *PIO_RESOURCE_DESCRIPTOR;


typedef struct _IO_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;

    ULONG Count;
    IO_RESOURCE_DESCRIPTOR Descriptors[1];
} IO_RESOURCE_LIST, *PIO_RESOURCE_LIST;

typedef struct _IO_RESOURCE_REQUIREMENTS_LIST {
    ULONG ListSize;
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    ULONG SlotNumber;
    ULONG Reserved[3];
    ULONG AlternativeLists;
    IO_RESOURCE_LIST  List[1];
} IO_RESOURCE_REQUIREMENTS_LIST, *PIO_RESOURCE_REQUIREMENTS_LIST;

typedef struct _POWER_SEQUENCE {
    ULONG SequenceD1;
    ULONG SequenceD2;
    ULONG SequenceD3;
} POWER_SEQUENCE, *PPOWER_SEQUENCE;

typedef struct _SYSTEM_POWER_STATE_CONTEXT {
    union {
        struct {
            ULONG   Reserved1             : 8;
            ULONG   TargetSystemState     : 4;
            ULONG   EffectiveSystemState  : 4;
            ULONG   CurrentSystemState    : 4;
            ULONG   IgnoreHibernationPath : 1;
            ULONG   Reserved2             : 11;
        };
        ULONG ContextAsUlong;
    };
} SYSTEM_POWER_STATE_CONTEXT, *PSYSTEM_POWER_STATE_CONTEXT;

typedef union _POWER_STATE {
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;

#pragma pack(push,4)

typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Generic;

        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        
        

        struct {
            ULONG Level;
            ULONG Vector;
            KAFFINITY Affinity;
        } Interrupt;

        
        
        
        

        struct {
            union {
               struct {
                   USHORT Reserved;
                   USHORT MessageCount;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Raw;

               struct {
                   ULONG Level;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Translated;
            };
        } MessageInterrupt;
        
        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;    
            ULONG Length;
        } Memory;

        
        
        

        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        
        
        
        

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Start;
            ULONG Length;
            ULONG Reserved;
        } BusNumber;

        
        
        
        
        
        

        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;

        
        
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length40;
        } Memory40;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length48;
        } Memory48;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length64;
        } Memory64;
        
        
    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;

#pragma pack(pop)

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;

typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;

typedef struct _CM_RESOURCE_LIST {
    ULONG Count;
    CM_FULL_RESOURCE_DESCRIPTOR List[1];
} CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;

typedef
NTSTATUS
IO_COMPLETION_ROUTINE (
     PDEVICE_OBJECT DeviceObject,
     PIRP Irp,
     PVOID Context
    );

typedef IO_COMPLETION_ROUTINE *PIO_COMPLETION_ROUTINE;

#pragma pack(push,4)

typedef struct _IO_STACK_LOCATION {
    UCHAR MajorFunction;
    UCHAR MinorFunction;
    UCHAR Flags;
    UCHAR Control;

    
    
    
    
    

    union {

        
        
        

        struct {
            PIO_SECURITY_CONTEXT SecurityContext;
            ULONG Options;
            USHORT  FileAttributes;
            USHORT ShareAccess;
            ULONG  EaLength;
        } Create;


        
        
        

        struct {
            ULONG Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } Read;

        
        
        

        struct {
            ULONG Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } Write;

        
        
        

        struct {
            ULONG Length;
            PUNICODE_STRING FileName;
            FILE_INFORMATION_CLASS FileInformationClass;
            ULONG  FileIndex;
        } QueryDirectory;

        
        
        

        struct {
            ULONG Length;
            ULONG  CompletionFilter;
        } NotifyDirectory;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS  FileInformationClass;
        } QueryFile;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS  FileInformationClass;
            PFILE_OBJECT FileObject;
            union {
                struct {
                    BOOLEAN ReplaceIfExists;
                    BOOLEAN AdvanceOnly;
                };
                ULONG ClusterCount;
                HANDLE DeleteHandle;
            };
        } SetFile;



        
        
        

        struct {
            ULONG Length;
            PVOID EaList;
            ULONG EaListLength;
            ULONG  EaIndex;
        } QueryEa;

        
        
        

        struct {
            ULONG Length;
        } SetEa;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS  FsInformationClass;
        } QueryVolume;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS  FsInformationClass;
        } SetVolume;

        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG  InputBufferLength;
            ULONG  FsControlCode;
            PVOID Type3InputBuffer;
        } FileSystemControl;
        
        
        

        struct {
            PLARGE_INTEGER Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } LockControl;

        
        
        
        
        



        
        
        
        
        



        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG  InputBufferLength;
            ULONG  IoControlCode;
            PVOID Type3InputBuffer;
        } DeviceIoControl;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            ULONG  Length;
        } QuerySecurity;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            PSECURITY_DESCRIPTOR SecurityDescriptor;
        } SetSecurity;

        
        
        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } MountVolume;

        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } VerifyVolume;

        
        
        

        struct {
            struct _SCSI_REQUEST_BLOCK *Srb;
        } Scsi;



        
        
        

        struct {
            ULONG Length;
            PSID StartSid;
            PFILE_GET_QUOTA_INFORMATION SidList;
            ULONG SidListLength;
        } QueryQuota;

        
        
        

        struct {
            ULONG Length;
        } SetQuota;



        
        
        

        struct {
            DEVICE_RELATION_TYPE Type;
        } QueryDeviceRelations;

        
        
        

        struct {
            const GUID *InterfaceType;
            USHORT Size;
            USHORT Version;
            PINTERFACE Interface;
            PVOID InterfaceSpecificData;
        } QueryInterface;

        
        
        

        struct {
            PDEVICE_CAPABILITIES Capabilities;
        } DeviceCapabilities;

        
        
        

        struct {
            PIO_RESOURCE_REQUIREMENTS_LIST IoResourceRequirementList;
        } FilterResourceRequirements;

        
        
        

        struct {
            ULONG WhichSpace;
            PVOID Buffer;
            ULONG Offset;
            ULONG  Length;
        } ReadWriteConfig;

        
        
        

        struct {
            BOOLEAN Lock;
        } SetLock;

        
        
        

        struct {
            BUS_QUERY_ID_TYPE IdType;
        } QueryId;

        
        
        

        struct {
            DEVICE_TEXT_TYPE DeviceTextType;
            LCID  LocaleId;
        } QueryDeviceText;

        
        
        

        struct {
            BOOLEAN InPath;
            BOOLEAN Reserved[3];
            DEVICE_USAGE_NOTIFICATION_TYPE  Type;
        } UsageNotification;

        
        
        

        struct {
            SYSTEM_POWER_STATE PowerState;
        } WaitWake;

        
        
        

        struct {
            PPOWER_SEQUENCE PowerSequence;
        } PowerSequence;

        
        
        


        struct {
            union {
                ULONG SystemContext;
                SYSTEM_POWER_STATE_CONTEXT SystemPowerStateContext;
            };
            POWER_STATE_TYPE  Type;
            POWER_STATE  State;
            POWER_ACTION  ShutdownType;
        } Power;








        
        
        

        struct {
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
        } StartDevice;

        
        
        
        
        

        
        
        

        struct {
            ULONG_PTR ProviderId;
            PVOID DataPath;
            ULONG BufferSize;
            PVOID Buffer;
        } WMI;

        
        
        

        struct {
            PVOID Argument1;
            PVOID Argument2;
            PVOID Argument3;
            PVOID Argument4;
        } Others;

    } Parameters;

    
    
    
    

    PDEVICE_OBJECT DeviceObject;

    
    
    
    

    PFILE_OBJECT FileObject;

    
    
    
    

    PIO_COMPLETION_ROUTINE CompletionRoutine;

    
    
    
    

    PVOID Context;

} IO_STACK_LOCATION, *PIO_STACK_LOCATION;
#pragma pack(pop)

#pragma pack(pop)


#elif CST_OSVER == MACRO_OSVER_WIN7 // #elif CST_OSVER == MACRO_OSVER_LH


// ***** Kernel data types for Win7 x86 *****
// ------------------------------------------

#pragma pack(push,8)

typedef short CSHORT;

typedef CCHAR KPROCESSOR_MODE;

typedef UCHAR KIRQL;


typedef enum {
    BusQueryDeviceID = 0,       
    BusQueryHardwareIDs = 1,    
    BusQueryCompatibleIDs = 2,  
    BusQueryInstanceID = 3,     
    BusQueryDeviceSerialNumber = 4,   
    BusQueryContainerID = 5     
} BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;

typedef enum _DEVICE_RELATION_TYPE {
    BusRelations,
    EjectionRelations,
    PowerRelations,
    RemovalRelations,
    TargetDeviceRelation,
    SingleBusRelations,
    TransportRelations
} DEVICE_RELATION_TYPE, *PDEVICE_RELATION_TYPE;

typedef enum {
    DeviceTextDescription = 0,            
    DeviceTextLocationInformation = 1     
} DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;

typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE {
    DeviceUsageTypeUndefined,
    DeviceUsageTypePaging,
    DeviceUsageTypeHibernation,
    DeviceUsageTypeDumpFile
} DEVICE_USAGE_NOTIFICATION_TYPE;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   
    FileBothDirectoryInformation,   
    FileBasicInformation,           
    FileStandardInformation,        
    FileInternalInformation,        
    FileEaInformation,              
    FileAccessInformation,          
    FileNameInformation,            
    FileRenameInformation,          
    FileLinkInformation,            
    FileNamesInformation,           
    FileDispositionInformation,     
    FilePositionInformation,        
    FileFullEaInformation,          
    FileModeInformation,            
    FileAlignmentInformation,       
    FileAllInformation,             
    FileAllocationInformation,      
    FileEndOfFileInformation,       
    FileAlternateNameInformation,   
    FileStreamInformation,          
    FilePipeInformation,            
    FilePipeLocalInformation,       
    FilePipeRemoteInformation,      
    FileMailslotQueryInformation,   
    FileMailslotSetInformation,     
    FileCompressionInformation,     
    FileObjectIdInformation,        
    FileCompletionInformation,      
    FileMoveClusterInformation,     
    FileQuotaInformation,           
    FileReparsePointInformation,    
    FileNetworkOpenInformation,     
    FileAttributeTagInformation,    
    FileTrackingInformation,        
    FileIdBothDirectoryInformation, 
    FileIdFullDirectoryInformation, 
    FileValidDataLengthInformation, 
    FileShortNameInformation,       
    FileIoCompletionNotificationInformation, 
    FileIoStatusBlockRangeInformation,       
    FileIoPriorityHintInformation,           
    FileSfioReserveInformation,              
    FileSfioVolumeInformation,               
    FileHardLinkInformation,                 
    FileProcessIdsUsingFileInformation,      
    FileNormalizedNameInformation,           
    FileNetworkPhysicalNameInformation,      
    FileIdGlobalTxDirectoryInformation,      
    FileIsRemoteDeviceInformation,           
    FileAttributeCacheInformation,           
    FileNumaNodeInformation,                 
    FileStandardLinkInformation,             
    FileRemoteProtocolInformation,           
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef enum _FSINFOCLASS {
    FileFsVolumeInformation       = 1,
    FileFsLabelInformation,      
    FileFsSizeInformation,       
    FileFsDeviceInformation,     
    FileFsAttributeInformation,  
    FileFsControlInformation,    
    FileFsFullSizeInformation,   
    FileFsObjectIdInformation,   
    FileFsDriverPathInformation, 
    FileFsVolumeFlagsInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    Vmcs,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef enum _IO_ALLOCATION_ACTION {
    KeepObject = 1,
    DeallocateObject,
    DeallocateObjectKeepRegisters
} IO_ALLOCATION_ACTION, *PIO_ALLOCATION_ACTION;

typedef enum _IRQ_DEVICE_POLICY {
    IrqPolicyMachineDefault = 0,
    IrqPolicyAllCloseProcessors,
    IrqPolicyOneCloseProcessor,
    IrqPolicyAllProcessorsInMachine,
    IrqPolicySpecifiedProcessors,
    IrqPolicySpreadMessagesAcrossAllProcessors
} IRQ_DEVICE_POLICY, *PIRQ_DEVICE_POLICY;

typedef enum _IRQ_PRIORITY {
    IrqPriorityUndefined = 0,
    IrqPriorityLow,
    IrqPriorityNormal,
    IrqPriorityHigh
} IRQ_PRIORITY, *PIRQ_PRIORITY;

typedef enum _POWER_STATE_TYPE {
    SystemPowerState = 0,
    DevicePowerState
} POWER_STATE_TYPE, *PPOWER_STATE_TYPE;

typedef struct _SECURITY_SUBJECT_CONTEXT {
    PACCESS_TOKEN ClientToken;
    SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
    PACCESS_TOKEN PrimaryToken;
    PVOID ProcessAuditId;
    } SECURITY_SUBJECT_CONTEXT, *PSECURITY_SUBJECT_CONTEXT;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
     PWCH   Buffer;
} UNICODE_STRING;

typedef UNICODE_STRING *PUNICODE_STRING;

typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _INITIAL_PRIVILEGE_SET {
    ULONG PrivilegeCount;
    ULONG Control;
    LUID_AND_ATTRIBUTES Privilege[3];
    } INITIAL_PRIVILEGE_SET, * PINITIAL_PRIVILEGE_SET;

typedef struct _ACCESS_STATE {
   LUID OperationID;                
   BOOLEAN SecurityEvaluated;
   BOOLEAN GenerateAudit;
   BOOLEAN GenerateOnClose;
   BOOLEAN PrivilegesAllocated;
   ULONG Flags;
   ACCESS_MASK RemainingDesiredAccess;
   ACCESS_MASK PreviouslyGrantedAccess;
   ACCESS_MASK OriginalDesiredAccess;
   SECURITY_SUBJECT_CONTEXT SubjectSecurityContext;
   PSECURITY_DESCRIPTOR SecurityDescriptor; 
   PVOID AuxData;
   union {
      INITIAL_PRIVILEGE_SET InitialPrivilegeSet;
      PRIVILEGE_SET PrivilegeSet;
      } Privileges;

   BOOLEAN AuditPrivileges;
   UNICODE_STRING ObjectName;
   UNICODE_STRING ObjectTypeName;

   } ACCESS_STATE, *PACCESS_STATE;

typedef struct _IO_SECURITY_CONTEXT {
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_STATE AccessState;
    ACCESS_MASK DesiredAccess;
    ULONG FullCreateOptions;
} IO_SECURITY_CONTEXT, *PIO_SECURITY_CONTEXT;

typedef struct _IO_TIMER *PIO_TIMER;

typedef struct _VPB {
    CSHORT Type;
    CSHORT Size;
    USHORT Flags;
    USHORT VolumeLabelLength; 
    struct _DEVICE_OBJECT *DeviceObject;
    struct _DEVICE_OBJECT *RealDevice;
    ULONG SerialNumber;
    ULONG ReferenceCount;
    WCHAR VolumeLabel[(32 * sizeof(WCHAR)) / sizeof(WCHAR)];
} VPB, *PVPB;

typedef struct _KDEVICE_QUEUE_ENTRY {
    LIST_ENTRY DeviceListEntry;
    ULONG SortKey;
    BOOLEAN Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY, *PRKDEVICE_QUEUE_ENTRY;

typedef
IO_ALLOCATION_ACTION
DRIVER_CONTROL (
     struct _DEVICE_OBJECT *DeviceObject,
     struct _IRP *Irp,
     PVOID MapRegisterBase,
     PVOID Context
    );
typedef DRIVER_CONTROL *PDRIVER_CONTROL;

typedef
void
KDEFERRED_ROUTINE (
     struct _KDPC *Dpc,
     PVOID DeferredContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );

 typedef KDEFERRED_ROUTINE *PKDEFERRED_ROUTINE;

 typedef struct _KDPC {
    UCHAR Type;
    UCHAR Importance;
    volatile USHORT Number;
    LIST_ENTRY DpcListEntry;
    PKDEFERRED_ROUTINE DeferredRoutine;
    PVOID DeferredContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
     PVOID DpcData;
} KDPC, *PKDPC, *PRKDPC;

typedef struct _WAIT_CONTEXT_BLOCK {
    KDEVICE_QUEUE_ENTRY WaitQueueEntry;
    PDRIVER_CONTROL DeviceRoutine;
    PVOID DeviceContext;
    ULONG NumberOfMapRegisters;
    PVOID DeviceObject;
    PVOID CurrentIrp;
    PKDPC BufferChainingDpc;
} WAIT_CONTEXT_BLOCK, *PWAIT_CONTEXT_BLOCK;

typedef struct _KDEVICE_QUEUE {
    CSHORT Type;
    CSHORT Size;
    LIST_ENTRY DeviceListHead;
    KSPIN_LOCK Lock;
    BOOLEAN Busy;
} KDEVICE_QUEUE, *PKDEVICE_QUEUE, *PRKDEVICE_QUEUE;

typedef struct _DISPATCHER_HEADER {
    union {
        struct {
            UCHAR Type;                 

            union {
                union {                 
                    UCHAR TimerControlFlags;
                    struct {
                        UCHAR Absolute              : 1;
                        UCHAR Coalescable           : 1;
                        UCHAR KeepShifting          : 1;    
                        UCHAR EncodedTolerableDelay : 5;    
                    } ;
                } ;

                UCHAR Abandoned;        
                BOOLEAN Signalling;     
            } ;

            union {
                union {
                    UCHAR ThreadControlFlags;  
                    struct {
                        UCHAR CpuThrottled      : 1;
                        UCHAR CycleProfiling    : 1;
                        UCHAR CounterProfiling  : 1;
                        UCHAR Reserved          : 5;
                    } ;
                } ;
                UCHAR Hand;             
                UCHAR Size;             
            } ;

            union {
                union {                 
                    UCHAR TimerMiscFlags;
                    struct {
                        UCHAR Index             : 1;
                        UCHAR Processor         : 5;
                        UCHAR Inserted          : 1;
                        volatile UCHAR Expired  : 1;
                    } ;
                } ;
                union {                 
                    BOOLEAN DebugActive;
                    struct {
                        BOOLEAN ActiveDR7       : 1;
                        BOOLEAN Instrumented    : 1;
                        BOOLEAN Reserved2       : 4;
                        BOOLEAN UmsScheduled    : 1;
                        BOOLEAN UmsPrimary      : 1;
                    } ;
                } ;
                BOOLEAN DpcActive;      
            } ;
        } ;

        volatile LONG Lock;             
    } ;

    LONG SignalState;                   
    LIST_ENTRY WaitListHead;            
} DISPATCHER_HEADER;

typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;

typedef struct __declspec(align(8)) _DEVICE_OBJECT {
    CSHORT Type;
    USHORT Size;
    LONG ReferenceCount;
    struct _DRIVER_OBJECT *DriverObject;
    struct _DEVICE_OBJECT *NextDevice;
    struct _DEVICE_OBJECT *AttachedDevice;
    struct _IRP *CurrentIrp;
    PIO_TIMER Timer;
    ULONG Flags;                                
    ULONG Characteristics;                      
     PVPB Vpb;
    PVOID DeviceExtension;
    ULONG DeviceType;
    CCHAR StackSize;
    union {
        LIST_ENTRY ListEntry;
        WAIT_CONTEXT_BLOCK Wcb;
    } Queue;
    ULONG AlignmentRequirement;
    KDEVICE_QUEUE DeviceQueue;
    KDPC Dpc;

    
    
    
    

    ULONG ActiveThreadCount;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    KEVENT DeviceLock;

    USHORT SectorSize;
    USHORT Spare1;

    struct _DEVOBJ_EXTENSION  *DeviceObjectExtension;
    PVOID  Reserved;

} DEVICE_OBJECT;

typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT; 

typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS;
typedef SECTION_OBJECT_POINTERS *PSECTION_OBJECT_POINTERS;

typedef struct _IO_COMPLETION_CONTEXT {
    PVOID Port;
    PVOID Key;
} IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;

typedef struct _FILE_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    PVPB Vpb;
    PVOID FsContext;
    PVOID FsContext2;
    PSECTION_OBJECT_POINTERS SectionObjectPointer;
    PVOID PrivateCacheMap;
    NTSTATUS FinalStatus;
    struct _FILE_OBJECT *RelatedFileObject;
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    ULONG Flags;
    UNICODE_STRING FileName;
    LARGE_INTEGER CurrentByteOffset;
     ULONG Waiters;
     ULONG Busy;
    PVOID LastLock;
    KEVENT Lock;
    KEVENT Event;
     PIO_COMPLETION_CONTEXT CompletionContext;
    KSPIN_LOCK IrpListLock;
    LIST_ENTRY IrpList;
     PVOID FileObjectExtension;
} FILE_OBJECT;
typedef struct _FILE_OBJECT *PFILE_OBJECT; 

typedef struct _FILE_GET_QUOTA_INFORMATION *PFILE_GET_QUOTA_INFORMATION;

typedef void (*PINTERFACE_REFERENCE)(PVOID Context);

typedef void (*PINTERFACE_DEREFERENCE)(PVOID Context);

#undef INTERFACE

typedef struct _INTERFACE {
    USHORT Size;
    USHORT Version;
    PVOID Context;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;
    
} INTERFACE, *PINTERFACE;

typedef  struct _DEVICE_CAPABILITIES {
    USHORT Size;
    USHORT Version;  
    ULONG DeviceD1:1;
    ULONG DeviceD2:1;
    ULONG LockSupported:1;
    ULONG EjectSupported:1; 
    ULONG Removable:1;
    ULONG DockDevice:1;
    ULONG UniqueID:1;
    ULONG SilentInstall:1;
    ULONG RawDeviceOK:1;
    ULONG SurpriseRemovalOK:1;
    ULONG WakeFromD0:1;
    ULONG WakeFromD1:1;
    ULONG WakeFromD2:1;
    ULONG WakeFromD3:1;
    ULONG HardwareDisabled:1;
    ULONG NonDynamic:1;
    ULONG WarmEjectSupported:1;
    ULONG NoDisplayInUI:1;
    ULONG Reserved1:1;
    ULONG Reserved:13;

    ULONG Address;
    ULONG UINumber;

    DEVICE_POWER_STATE DeviceState[7];
    SYSTEM_POWER_STATE SystemWake;
    DEVICE_POWER_STATE DeviceWake;
    ULONG D1Latency;
    ULONG D2Latency;
    ULONG D3Latency;
} DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;


typedef struct _IO_RESOURCE_DESCRIPTOR {
    UCHAR Option;
    UCHAR Type;                         
    UCHAR ShareDisposition;             
    UCHAR Spare1;
    USHORT Flags;                       
    USHORT Spare2;                      

    union {
        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Port;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory;

        struct {
            ULONG MinimumVector;
            ULONG MaximumVector;



            IRQ_DEVICE_POLICY AffinityPolicy;
            IRQ_PRIORITY PriorityPolicy;
            KAFFINITY TargetedProcessors;
        } Interrupt;

        struct {
            ULONG MinimumChannel;
            ULONG MaximumChannel;
        } Dma;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Generic;

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Length;
            ULONG MinBusNumber;
            ULONG MaxBusNumber;
            ULONG Reserved;
        } BusNumber;

        struct {
            ULONG Priority;   
            ULONG Reserved1;
            ULONG Reserved2;
        } ConfigData;

        
        
        
        

        struct {
            ULONG Length40;
            ULONG Alignment40;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory40;

        struct {
            ULONG Length48;
            ULONG Alignment48;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory48;

        struct {
            ULONG Length64;
            ULONG Alignment64;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory64;


    } u;

} IO_RESOURCE_DESCRIPTOR, *PIO_RESOURCE_DESCRIPTOR;

typedef struct _IO_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;

    ULONG Count;
    IO_RESOURCE_DESCRIPTOR Descriptors[1];
} IO_RESOURCE_LIST, *PIO_RESOURCE_LIST;

typedef struct _IO_RESOURCE_REQUIREMENTS_LIST {
    ULONG ListSize;
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    ULONG SlotNumber;
    ULONG Reserved[3];
    ULONG AlternativeLists;
    IO_RESOURCE_LIST  List[1];
} IO_RESOURCE_REQUIREMENTS_LIST, *PIO_RESOURCE_REQUIREMENTS_LIST;

typedef struct _POWER_SEQUENCE {
    ULONG SequenceD1;
    ULONG SequenceD2;
    ULONG SequenceD3;
} POWER_SEQUENCE, *PPOWER_SEQUENCE;

typedef struct _SYSTEM_POWER_STATE_CONTEXT {
    union {
        struct {
            ULONG   Reserved1             : 8;
            ULONG   TargetSystemState     : 4;
            ULONG   EffectiveSystemState  : 4;
            ULONG   CurrentSystemState    : 4;
            ULONG   IgnoreHibernationPath : 1;
            ULONG   PseudoTransition      : 1;
            ULONG   Reserved2             : 10;
        } ;

        ULONG ContextAsUlong;
    } ;
} SYSTEM_POWER_STATE_CONTEXT, *PSYSTEM_POWER_STATE_CONTEXT;

typedef union _POWER_STATE {
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;

#pragma pack(push,4)

typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Generic;

        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        
        

        struct {



            ULONG Level;
            ULONG Vector;
            KAFFINITY Affinity;
        } Interrupt;

        
        
        
        

        struct {
            union {
               struct {


                   USHORT Reserved;
                   USHORT MessageCount;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Raw;

               struct {



                   ULONG Level;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Translated;
            } ;
        } MessageInterrupt;

        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;    
            ULONG Length;
        } Memory;

        
        
        

        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        
        
        
        

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Start;
            ULONG Length;
            ULONG Reserved;
        } BusNumber;

        
        
        
        
        
        

        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;

        
        
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length40;
        } Memory40;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length48;
        } Memory48;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length64;
        } Memory64;


    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;

#pragma pack(pop)

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;

typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;


typedef struct _CM_RESOURCE_LIST {
    ULONG Count;
    CM_FULL_RESOURCE_DESCRIPTOR List[1];
} CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;

typedef  struct _MDL {
    struct _MDL *Next;
    CSHORT Size;
    CSHORT MdlFlags;
    struct _EPROCESS *Process;
    PVOID MappedSystemVa;
    PVOID StartVa;
    ULONG ByteCount;
    ULONG ByteOffset;
} MDL, *PMDL;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    } ;

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef
void
(__stdcall *PIO_APC_ROUTINE) (
     PVOID ApcContext,
     PIO_STATUS_BLOCK IoStatusBlock,
     ULONG Reserved
    );

typedef
void
DRIVER_CANCEL (
     struct _DEVICE_OBJECT *DeviceObject,
       struct _IRP *Irp
    );

typedef DRIVER_CANCEL *PDRIVER_CANCEL;

typedef struct _ETHREAD *PETHREAD;

typedef
void
KNORMAL_ROUTINE (
     PVOID NormalContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );
typedef KNORMAL_ROUTINE *PKNORMAL_ROUTINE;

typedef
void
KKERNEL_ROUTINE (
     struct _KAPC *Apc,
     PKNORMAL_ROUTINE *NormalRoutine,
     PVOID *NormalContext,
     PVOID *SystemArgument1,
     PVOID *SystemArgument2
    );
typedef KKERNEL_ROUTINE *PKKERNEL_ROUTINE;

typedef
void
KRUNDOWN_ROUTINE (
     struct _KAPC *Apc
    );
typedef KRUNDOWN_ROUTINE *PKRUNDOWN_ROUTINE;

typedef struct _KAPC {
    UCHAR Type;
    UCHAR SpareByte0;
    UCHAR Size;
    UCHAR SpareByte1;
    ULONG SpareLong0;
    struct _KTHREAD *Thread;
    LIST_ENTRY ApcListEntry;
    PKKERNEL_ROUTINE KernelRoutine;
    PKRUNDOWN_ROUTINE RundownRoutine;
    PKNORMAL_ROUTINE NormalRoutine;
    PVOID NormalContext;

    
    
    

    PVOID SystemArgument1;
    PVOID SystemArgument2;
    CCHAR ApcStateIndex;
    KPROCESSOR_MODE ApcMode;
    BOOLEAN Inserted;
} KAPC, *PKAPC, *PRKAPC;

typedef struct __declspec(align(8)) _IRP {
    CSHORT Type;
    USHORT Size;

    
    
    

    
    
    
    

    PMDL MdlAddress;

    
    
    

    ULONG Flags;

    
    
    
    
    
    
    
    
    
    
    
    
    

    union {
        struct _IRP *MasterIrp;
         LONG IrpCount;
        PVOID SystemBuffer;
    } AssociatedIrp;

    
    
    
    

    LIST_ENTRY ThreadListEntry;

    
    
    

    IO_STATUS_BLOCK IoStatus;

    
    
    

    KPROCESSOR_MODE RequestorMode;

    
    
    
    

    BOOLEAN PendingReturned;

    
    
    

    CHAR StackCount;
    CHAR CurrentLocation;

    
    
    

    BOOLEAN Cancel;

    
    
    

    KIRQL CancelIrql;

    
    
    
    

    CCHAR ApcEnvironment;

    
    
    

    UCHAR AllocationFlags;

    
    
    

    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;
    union {
        struct {
            union {
                PIO_APC_ROUTINE UserApcRoutine;
                PVOID IssuingProcess;
            };
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    
    
    
    

     PDRIVER_CANCEL CancelRoutine;

    
    
    
    
    
    
    

    PVOID UserBuffer;

    
    
    
    
    
    
    
    
    

    union {

        struct {

            union {

                
                
                
                

                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;

                struct {

                    
                    
                    
                    
                    

                    PVOID DriverContext[4];

                } ;

            } ;

            
            
            

            PETHREAD Thread;

            
            
            
            
            

            PCHAR AuxiliaryBuffer;

            
            
            
            
            

            struct {

                
                
                
                

                LIST_ENTRY ListEntry;

                union {

                    
                    
                    
                    
                    
                    

                    struct _IO_STACK_LOCATION *CurrentStackLocation;

                    
                    
                    

                    ULONG PacketType;
                };
            };

            
            
            
            
            

            PFILE_OBJECT OriginalFileObject;

        } Overlay;

        
        
        
        
        
        
        

        KAPC Apc;

        
        
        
        

        PVOID CompletionKey;

    } Tail;

} IRP;

typedef IRP *PIRP;

typedef
NTSTATUS
IO_COMPLETION_ROUTINE (
     PDEVICE_OBJECT DeviceObject,
     PIRP Irp,
     PVOID Context
    );

typedef IO_COMPLETION_ROUTINE *PIO_COMPLETION_ROUTINE;

#pragma pack(push,4)

typedef struct _IO_STACK_LOCATION {
    UCHAR MajorFunction;
    UCHAR MinorFunction;
    UCHAR Flags;
    UCHAR Control;

    
    
    
    
    

    union {

        
        
        

        struct {
            PIO_SECURITY_CONTEXT SecurityContext;
            ULONG Options;
            USHORT  FileAttributes;
            USHORT ShareAccess;
            ULONG  EaLength;
        } Create;


        
        
        

        struct {
            ULONG Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } Read;

        
        
        

        struct {
            ULONG Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } Write;

        
        
        

        struct {
            ULONG Length;
            PUNICODE_STRING FileName;
            FILE_INFORMATION_CLASS FileInformationClass;
            ULONG  FileIndex;
        } QueryDirectory;

        
        
        

        struct {
            ULONG Length;
            ULONG  CompletionFilter;
        } NotifyDirectory;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS  FileInformationClass;
        } QueryFile;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS  FileInformationClass;
            PFILE_OBJECT FileObject;
            union {
                struct {
                    BOOLEAN ReplaceIfExists;
                    BOOLEAN AdvanceOnly;
                };
                ULONG ClusterCount;
                HANDLE DeleteHandle;
            };
        } SetFile;



        
        
        

        struct {
            ULONG Length;
            PVOID EaList;
            ULONG EaListLength;
            ULONG  EaIndex;
        } QueryEa;

        
        
        

        struct {
            ULONG Length;
        } SetEa;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS  FsInformationClass;
        } QueryVolume;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS  FsInformationClass;
        } SetVolume;

        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG  InputBufferLength;
            ULONG  FsControlCode;
            PVOID Type3InputBuffer;
        } FileSystemControl;
        
        
        

        struct {
            PLARGE_INTEGER Length;
            ULONG  Key;
            LARGE_INTEGER ByteOffset;
        } LockControl;

        
        
        
        
        



        
        
        
        
        



        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG  InputBufferLength;
            ULONG  IoControlCode;
            PVOID Type3InputBuffer;
        } DeviceIoControl;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            ULONG  Length;
        } QuerySecurity;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            PSECURITY_DESCRIPTOR SecurityDescriptor;
        } SetSecurity;

        
        
        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } MountVolume;

        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } VerifyVolume;

        
        
        

        struct {
            struct _SCSI_REQUEST_BLOCK *Srb;
        } Scsi;



        
        
        

        struct {
            ULONG Length;
            PSID StartSid;
            PFILE_GET_QUOTA_INFORMATION SidList;
            ULONG SidListLength;
        } QueryQuota;

        
        
        

        struct {
            ULONG Length;
        } SetQuota;



        
        
        

        struct {
            DEVICE_RELATION_TYPE Type;
        } QueryDeviceRelations;

        
        
        

        struct {
            const GUID *InterfaceType;
            USHORT Size;
            USHORT Version;
            PINTERFACE Interface;
            PVOID InterfaceSpecificData;
        } QueryInterface;

        
        
        

        struct {
            PDEVICE_CAPABILITIES Capabilities;
        } DeviceCapabilities;

        
        
        

        struct {
            PIO_RESOURCE_REQUIREMENTS_LIST IoResourceRequirementList;
        } FilterResourceRequirements;

        
        
        

        struct {
            ULONG WhichSpace;
            PVOID Buffer;
            ULONG Offset;
            ULONG  Length;
        } ReadWriteConfig;

        
        
        

        struct {
            BOOLEAN Lock;
        } SetLock;

        
        
        

        struct {
            BUS_QUERY_ID_TYPE IdType;
        } QueryId;

        
        
        

        struct {
            DEVICE_TEXT_TYPE DeviceTextType;
            LCID  LocaleId;
        } QueryDeviceText;

        
        
        

        struct {
            BOOLEAN InPath;
            BOOLEAN Reserved[3];
            DEVICE_USAGE_NOTIFICATION_TYPE  Type;
        } UsageNotification;

        
        
        

        struct {
            SYSTEM_POWER_STATE PowerState;
        } WaitWake;

        
        
        

        struct {
            PPOWER_SEQUENCE PowerSequence;
        } PowerSequence;

        
        
        


        struct {
            union {
                ULONG SystemContext;
                SYSTEM_POWER_STATE_CONTEXT SystemPowerStateContext;
            };
            POWER_STATE_TYPE  Type;
            POWER_STATE  State;
            POWER_ACTION  ShutdownType;
        } Power;








        
        
        

        struct {
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
        } StartDevice;

        
        
        
        
        

        
        
        

        struct {
            ULONG_PTR ProviderId;
            PVOID DataPath;
            ULONG BufferSize;
            PVOID Buffer;
        } WMI;

        
        
        

        struct {
            PVOID Argument1;
            PVOID Argument2;
            PVOID Argument3;
            PVOID Argument4;
        } Others;

    } Parameters;

    
    
    
    

    PDEVICE_OBJECT DeviceObject;

    
    
    
    

    PFILE_OBJECT FileObject;

    
    
    
    

    PIO_COMPLETION_ROUTINE CompletionRoutine;

    
    
    
    

    PVOID Context;

} IO_STACK_LOCATION, *PIO_STACK_LOCATION;

#pragma pack(pop)

#pragma pack(pop)

#endif // #elif CST_OSVER == MACRO_OSVER_WIN7
#elif ARCHITECTURE == MACRO_ARCH_X64 // #if ARCHITECTURE == MACRO_ARCH_X86
#if CST_OSVER == MACRO_OSVER_LH

// ***** Kernel data types for Vista x64 *****
// -------------------------------------------


#pragma pack(push, 8)

typedef short CSHORT;

typedef enum {
    BusQueryDeviceID = 0,       
    BusQueryHardwareIDs = 1,    
    BusQueryCompatibleIDs = 2,  
    BusQueryInstanceID = 3,     
    BusQueryDeviceSerialNumber = 4    
} BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;

typedef enum _DEVICE_RELATION_TYPE {
    BusRelations,
    EjectionRelations,
    PowerRelations,
    RemovalRelations,
    TargetDeviceRelation,
    SingleBusRelations
} DEVICE_RELATION_TYPE, *PDEVICE_RELATION_TYPE;

typedef enum {
    DeviceTextDescription = 0,            
    DeviceTextLocationInformation = 1     
} DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;

typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE {
    DeviceUsageTypeUndefined,
    DeviceUsageTypePaging,
    DeviceUsageTypeHibernation,
    DeviceUsageTypeDumpFile
} DEVICE_USAGE_NOTIFICATION_TYPE;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   
    FileBothDirectoryInformation,   
    FileBasicInformation,           
    FileStandardInformation,        
    FileInternalInformation,        
    FileEaInformation,              
    FileAccessInformation,          
    FileNameInformation,            
    FileRenameInformation,          
    FileLinkInformation,            
    FileNamesInformation,           
    FileDispositionInformation,     
    FilePositionInformation,        
    FileFullEaInformation,          
    FileModeInformation,            
    FileAlignmentInformation,       
    FileAllInformation,             
    FileAllocationInformation,      
    FileEndOfFileInformation,       
    FileAlternateNameInformation,   
    FileStreamInformation,          
    FilePipeInformation,            
    FilePipeLocalInformation,       
    FilePipeRemoteInformation,      
    FileMailslotQueryInformation,   
    FileMailslotSetInformation,     
    FileCompressionInformation,     
    FileObjectIdInformation,        
    FileCompletionInformation,      
    FileMoveClusterInformation,     
    FileQuotaInformation,           
    FileReparsePointInformation,    
    FileNetworkOpenInformation,     
    FileAttributeTagInformation,    
    FileTrackingInformation,        
    FileIdBothDirectoryInformation, 
    FileIdFullDirectoryInformation, 
    FileValidDataLengthInformation, 
    FileShortNameInformation,       
    FileIoCompletionNotificationInformation, 
    FileIoStatusBlockRangeInformation,       
    FileIoPriorityHintInformation,           
    FileSfioReserveInformation,              
    FileSfioVolumeInformation,               
    FileHardLinkInformation,                 
    FileProcessIdsUsingFileInformation,      
    FileNormalizedNameInformation,           
    FileNetworkPhysicalNameInformation,      
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef enum _FSINFOCLASS {
    FileFsVolumeInformation       = 1,
    FileFsLabelInformation,      
    FileFsSizeInformation,       
    FileFsDeviceInformation,     
    FileFsAttributeInformation,  
    FileFsControlInformation,    
    FileFsFullSizeInformation,   
    FileFsObjectIdInformation,   
    FileFsDriverPathInformation, 
    FileFsVolumeFlagsInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef enum _IRQ_DEVICE_POLICY {
    IrqPolicyMachineDefault = 0,
    IrqPolicyAllCloseProcessors,
    IrqPolicyOneCloseProcessor,
    IrqPolicyAllProcessorsInMachine,
    IrqPolicySpecifiedProcessors,
    IrqPolicySpreadMessagesAcrossAllProcessors
} IRQ_DEVICE_POLICY, *PIRQ_DEVICE_POLICY;

typedef enum _IRQ_PRIORITY {
    IrqPriorityUndefined = 0,
    IrqPriorityLow,
    IrqPriorityNormal,
    IrqPriorityHigh
} IRQ_PRIORITY, *PIRQ_PRIORITY;

typedef enum _POWER_STATE_TYPE {
    SystemPowerState = 0,
    DevicePowerState
} POWER_STATE_TYPE, *PPOWER_STATE_TYPE;

typedef  struct _MDL {
    struct _MDL *Next;
    CSHORT Size;
    CSHORT MdlFlags;
    struct _EPROCESS *Process;
    PVOID MappedSystemVa;
    PVOID StartVa;
    ULONG ByteCount;
    ULONG ByteOffset;
} MDL, *PMDL;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef CCHAR KPROCESSOR_MODE;

typedef UCHAR KIRQL;

typedef struct _DISPATCHER_HEADER {
    union {
        struct {
            UCHAR Type;
            union {
                UCHAR Abandoned;
                UCHAR Absolute;
                UCHAR NpxIrql;
                BOOLEAN Signalling;
            };

            union {
                UCHAR Size;
                UCHAR Hand;
            };

            union {
                UCHAR Inserted;
                BOOLEAN DebugActive;
                BOOLEAN DpcActive;
            };
        };

        volatile LONG Lock;
    };

    LONG SignalState;
    LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;

typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;

typedef
void
(__stdcall *PIO_APC_ROUTINE) (
     PVOID ApcContext,
     PIO_STATUS_BLOCK IoStatusBlock,
     ULONG Reserved
    );

typedef
void
DRIVER_CANCEL (
     struct _DEVICE_OBJECT *DeviceObject,
     struct _IRP *Irp
    );

typedef DRIVER_CANCEL *PDRIVER_CANCEL;

typedef struct _KDEVICE_QUEUE_ENTRY {
    LIST_ENTRY DeviceListEntry;
    ULONG SortKey;
    BOOLEAN Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY, *PRKDEVICE_QUEUE_ENTRY;

typedef struct _ETHREAD *PETHREAD;

typedef struct _IO_TIMER *PIO_TIMER;

typedef struct _VPB *PVPB;

typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT; 

typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS;
typedef SECTION_OBJECT_POINTERS *PSECTION_OBJECT_POINTERS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;



     PWCH   Buffer;
} UNICODE_STRING;

typedef struct _IO_COMPLETION_CONTEXT {
    PVOID Port;
    PVOID Key;
} IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;

typedef struct _FILE_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    PVPB Vpb;
    PVOID FsContext;
    PVOID FsContext2;
    PSECTION_OBJECT_POINTERS SectionObjectPointer;
    PVOID PrivateCacheMap;
    NTSTATUS FinalStatus;
    struct _FILE_OBJECT *RelatedFileObject;
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    ULONG Flags;
    UNICODE_STRING FileName;
    LARGE_INTEGER CurrentByteOffset;
     ULONG Waiters;
     ULONG Busy;
    PVOID LastLock;
    KEVENT Lock;
    KEVENT Event;
     PIO_COMPLETION_CONTEXT CompletionContext;
    KSPIN_LOCK IrpListLock;
    LIST_ENTRY IrpList;
     PVOID FileObjectExtension;
} FILE_OBJECT;
typedef struct _FILE_OBJECT *PFILE_OBJECT; 

typedef
void
(*PKNORMAL_ROUTINE) (
     PVOID NormalContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );

typedef
void
(*PKKERNEL_ROUTINE) (
     struct _KAPC *Apc,
      PKNORMAL_ROUTINE *NormalRoutine,
      PVOID *NormalContext,
      PVOID *SystemArgument1,
      PVOID *SystemArgument2
    );

typedef
void
(*PKRUNDOWN_ROUTINE) (
     struct _KAPC *Apc
    );

typedef
BOOLEAN
(*PKSYNCHRONIZE_ROUTINE) (
     PVOID SynchronizeContext
    );

typedef
BOOLEAN
(*PKTRANSFER_ROUTINE) (
    void
    );

typedef struct _KAPC {
    UCHAR Type;
    UCHAR SpareByte0;
    UCHAR Size;
    UCHAR SpareByte1;
    ULONG SpareLong0;
    struct _KTHREAD *Thread;
    LIST_ENTRY ApcListEntry;
    PKKERNEL_ROUTINE KernelRoutine;
    PKRUNDOWN_ROUTINE RundownRoutine;
    PKNORMAL_ROUTINE NormalRoutine;
    PVOID NormalContext;

    
    
    

    PVOID SystemArgument1;
    PVOID SystemArgument2;
    CCHAR ApcStateIndex;
    KPROCESSOR_MODE ApcMode;
    BOOLEAN Inserted;
} KAPC, *PKAPC, *PRKAPC;

typedef struct __declspec(align(16)) _IRP {
    CSHORT Type;
    USHORT Size;

    
    
    

    
    
    
    

    PMDL MdlAddress;

    
    
    

    ULONG Flags;

    
    
    
    
    
    
    
    
    
    
    
    
    

    union {
        struct _IRP *MasterIrp;
         LONG IrpCount;
        PVOID SystemBuffer;
    } AssociatedIrp;

    
    
    
    

    LIST_ENTRY ThreadListEntry;

    
    
    

    IO_STATUS_BLOCK IoStatus;

    
    
    

    KPROCESSOR_MODE RequestorMode;

    
    
    
    

    BOOLEAN PendingReturned;

    
    
    

    CHAR StackCount;
    CHAR CurrentLocation;

    
    
    

    BOOLEAN Cancel;

    
    
    

    KIRQL CancelIrql;

    
    
    
    

    CCHAR ApcEnvironment;

    
    
    

    UCHAR AllocationFlags;

    
    
    

    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;
    union {
        struct {
            union {
                PIO_APC_ROUTINE UserApcRoutine;
                PVOID IssuingProcess;
            };
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    
    
    
    

     PDRIVER_CANCEL CancelRoutine;

    
    
    
    
    
    
    

    PVOID UserBuffer;

    
    
    
    
    
    
    
    
    

    union {

        struct {

            union {

                
                
                
                

                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;

                struct {

                    
                    
                    
                    
                    

                    PVOID DriverContext[4];

                } ;

            } ;

            
            
            

            PETHREAD Thread;

            
            
            
            
            

            PCHAR AuxiliaryBuffer;

            
            
            
            
            

            struct {

                
                
                
                

                LIST_ENTRY ListEntry;

                union {

                    
                    
                    
                    
                    
                    

                    struct _IO_STACK_LOCATION *CurrentStackLocation;

                    
                    
                    

                    ULONG PacketType;
                };
            };

            
            
            
            
            

            PFILE_OBJECT OriginalFileObject;

        } Overlay;

        
        
        
        
        
        
        

        KAPC Apc;

        
        
        
        

        PVOID CompletionKey;

    } Tail;

} IRP, *PIRP;

typedef struct _IO_STACK_LOCATION *PIO_STACK_LOCATION;

typedef struct _ACCESS_STATE *PACCESS_STATE;

typedef struct _IO_SECURITY_CONTEXT {
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_STATE AccessState;
    ACCESS_MASK DesiredAccess;
    ULONG FullCreateOptions;
} IO_SECURITY_CONTEXT, *PIO_SECURITY_CONTEXT;

typedef UNICODE_STRING *PUNICODE_STRING;

typedef struct _FILE_GET_QUOTA_INFORMATION *PFILE_GET_QUOTA_INFORMATION;

typedef void (*PINTERFACE_REFERENCE)(PVOID Context);

typedef void (*PINTERFACE_DEREFERENCE)(PVOID Context);

typedef struct _INTERFACE {
    USHORT Size;
    USHORT Version;
    PVOID Context;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;
    
} CST_INTERFACE, *PINTERFACE;

typedef  struct _DEVICE_CAPABILITIES {
    USHORT Size;
    USHORT Version;  
    ULONG DeviceD1:1;
    ULONG DeviceD2:1;
    ULONG LockSupported:1;
    ULONG EjectSupported:1; 
    ULONG Removable:1;
    ULONG DockDevice:1;
    ULONG UniqueID:1;
    ULONG SilentInstall:1;
    ULONG RawDeviceOK:1;
    ULONG SurpriseRemovalOK:1;
    ULONG WakeFromD0:1;
    ULONG WakeFromD1:1;
    ULONG WakeFromD2:1;
    ULONG WakeFromD3:1;
    ULONG HardwareDisabled:1;
    ULONG NonDynamic:1;
    ULONG WarmEjectSupported:1;
    ULONG NoDisplayInUI:1;
    ULONG Reserved:14;

    ULONG Address;
    ULONG UINumber;

    DEVICE_POWER_STATE DeviceState[7];
    SYSTEM_POWER_STATE SystemWake;
    DEVICE_POWER_STATE DeviceWake;
    ULONG D1Latency;
    ULONG D2Latency;
    ULONG D3Latency;
} DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

typedef struct _IO_RESOURCE_DESCRIPTOR {
    UCHAR Option;
    UCHAR Type;                         
    UCHAR ShareDisposition;             
    UCHAR Spare1;
    USHORT Flags;                       
    USHORT Spare2;                      

    union {
        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Port;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory;

        struct {
            ULONG MinimumVector;
            ULONG MaximumVector;
            IRQ_DEVICE_POLICY AffinityPolicy;
            IRQ_PRIORITY PriorityPolicy;
            KAFFINITY TargetedProcessors;
        } Interrupt;

        struct {
            ULONG MinimumChannel;
            ULONG MaximumChannel;
        } Dma;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Generic;

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Length;
            ULONG MinBusNumber;
            ULONG MaxBusNumber;
            ULONG Reserved;
        } BusNumber;

        struct {
            ULONG Priority;   
            ULONG Reserved1;
            ULONG Reserved2;
        } ConfigData;

        
        
        
        

        struct {
            ULONG Length40;
            ULONG Alignment40;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory40;

        struct {
            ULONG Length48;
            ULONG Alignment48;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory48;

        struct {
            ULONG Length64;
            ULONG Alignment64;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory64;


    } u;

} IO_RESOURCE_DESCRIPTOR, *PIO_RESOURCE_DESCRIPTOR;

typedef struct _IO_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;

    ULONG Count;
    IO_RESOURCE_DESCRIPTOR Descriptors[1];
} IO_RESOURCE_LIST, *PIO_RESOURCE_LIST;

typedef struct _IO_RESOURCE_REQUIREMENTS_LIST {
    ULONG ListSize;
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    ULONG SlotNumber;
    ULONG Reserved[3];
    ULONG AlternativeLists;
    IO_RESOURCE_LIST  List[1];
} IO_RESOURCE_REQUIREMENTS_LIST, *PIO_RESOURCE_REQUIREMENTS_LIST;

typedef struct _POWER_SEQUENCE {
    ULONG SequenceD1;
    ULONG SequenceD2;
    ULONG SequenceD3;
} POWER_SEQUENCE, *PPOWER_SEQUENCE;

typedef struct _SYSTEM_POWER_STATE_CONTEXT {
    union {
        struct {
            ULONG   Reserved1             : 8;
            ULONG   TargetSystemState     : 4;
            ULONG   EffectiveSystemState  : 4;
            ULONG   CurrentSystemState    : 4;
            ULONG   IgnoreHibernationPath : 1;
            ULONG   Reserved2             : 11;
        };
        ULONG ContextAsUlong;
    };
} SYSTEM_POWER_STATE_CONTEXT, *PSYSTEM_POWER_STATE_CONTEXT;

typedef union _POWER_STATE {
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;

#pragma pack(push, 4)
typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Generic;

        
        

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        
        

        struct {
            ULONG Level;
            ULONG Vector;
            KAFFINITY Affinity;
        } Interrupt;

        
        
        
        

        struct {
            union {
               struct {
                   USHORT Reserved;
                   USHORT MessageCount;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Raw;

               struct {
                   ULONG Level;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Translated;
            };
        } MessageInterrupt;
        
        
        
        
        
        

        struct {
            PHYSICAL_ADDRESS Start;    
            ULONG Length;
        } Memory;

        
        
        

        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        
        
        
        

        struct {
            ULONG Data[3];
        } DevicePrivate;

        
        
        

        struct {
            ULONG Start;
            ULONG Length;
            ULONG Reserved;
        } BusNumber;

        
        
        
        
        
        

        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;

        
        
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length40;
        } Memory40;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length48;
        } Memory48;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length64;
        } Memory64;
        
        
    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;
#pragma pack(pop)

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;

typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
    INTERFACE_TYPE InterfaceType; 
    ULONG BusNumber; 
    CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;

typedef struct _CM_RESOURCE_LIST {
    ULONG Count;
    CM_FULL_RESOURCE_DESCRIPTOR List[1];
} CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;

typedef
NTSTATUS
IO_COMPLETION_ROUTINE (
     PDEVICE_OBJECT DeviceObject,
     PIRP Irp,
     PVOID Context
    );

typedef IO_COMPLETION_ROUTINE *PIO_COMPLETION_ROUTINE;

typedef struct _IO_STACK_LOCATION {
    UCHAR MajorFunction;
    UCHAR MinorFunction;
    UCHAR Flags;
    UCHAR Control;

    
    
    
    
    

    union {

        
        
        

        struct {
            PIO_SECURITY_CONTEXT SecurityContext;
            ULONG Options;
            USHORT __declspec(align(8)) FileAttributes;
            USHORT ShareAccess;
            ULONG __declspec(align(8)) EaLength;
        } Create;


        
        
        

        struct {
            ULONG Length;
            ULONG __declspec(align(8)) Key;
            LARGE_INTEGER ByteOffset;
        } Read;

        
        
        

        struct {
            ULONG Length;
            ULONG __declspec(align(8)) Key;
            LARGE_INTEGER ByteOffset;
        } Write;

        
        
        

        struct {
            ULONG Length;
            PUNICODE_STRING FileName;
            FILE_INFORMATION_CLASS FileInformationClass;
            ULONG __declspec(align(8)) FileIndex;
        } QueryDirectory;

        
        
        

        struct {
            ULONG Length;
            ULONG __declspec(align(8)) CompletionFilter;
        } NotifyDirectory;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS __declspec(align(8)) FileInformationClass;
        } QueryFile;

        
        
        

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS __declspec(align(8)) FileInformationClass;
            PFILE_OBJECT FileObject;
            union {
                struct {
                    BOOLEAN ReplaceIfExists;
                    BOOLEAN AdvanceOnly;
                };
                ULONG ClusterCount;
                HANDLE DeleteHandle;
            };
        } SetFile;



        
        
        

        struct {
            ULONG Length;
            PVOID EaList;
            ULONG EaListLength;
            ULONG __declspec(align(8)) EaIndex;
        } QueryEa;

        
        
        

        struct {
            ULONG Length;
        } SetEa;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS __declspec(align(8)) FsInformationClass;
        } QueryVolume;



        
        
        

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS __declspec(align(8)) FsInformationClass;
        } SetVolume;

        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG __declspec(align(8)) InputBufferLength;
            ULONG __declspec(align(8)) FsControlCode;
            PVOID Type3InputBuffer;
        } FileSystemControl;
        
        
        

        struct {
            PLARGE_INTEGER Length;
            ULONG __declspec(align(8)) Key;
            LARGE_INTEGER ByteOffset;
        } LockControl;

        
        
        
        
        



        
        
        
        
        



        
        
        
        
        
        

        struct {
            ULONG OutputBufferLength;
            ULONG __declspec(align(8)) InputBufferLength;
            ULONG __declspec(align(8)) IoControlCode;
            PVOID Type3InputBuffer;
        } DeviceIoControl;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            ULONG __declspec(align(8)) Length;
        } QuerySecurity;

        
        
        

        struct {
            SECURITY_INFORMATION SecurityInformation;
            PSECURITY_DESCRIPTOR SecurityDescriptor;
        } SetSecurity;

        
        
        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } MountVolume;

        
        
        

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } VerifyVolume;

        
        
        

        struct {
            struct _SCSI_REQUEST_BLOCK *Srb;
        } Scsi;



        
        
        

        struct {
            ULONG Length;
            PSID StartSid;
            PFILE_GET_QUOTA_INFORMATION SidList;
            ULONG SidListLength;
        } QueryQuota;

        
        
        

        struct {
            ULONG Length;
        } SetQuota;



        
        
        

        struct {
            DEVICE_RELATION_TYPE Type;
        } QueryDeviceRelations;

        
        
        

        struct {
            const GUID *InterfaceType;
            USHORT Size;
            USHORT Version;
            PINTERFACE Interface;
            PVOID InterfaceSpecificData;
        } QueryInterface;

        
        
        

        struct {
            PDEVICE_CAPABILITIES Capabilities;
        } DeviceCapabilities;

        
        
        

        struct {
            PIO_RESOURCE_REQUIREMENTS_LIST IoResourceRequirementList;
        } FilterResourceRequirements;

        
        
        

        struct {
            ULONG WhichSpace;
            PVOID Buffer;
            ULONG Offset;
            ULONG __declspec(align(8)) Length;
        } ReadWriteConfig;

        
        
        

        struct {
            BOOLEAN Lock;
        } SetLock;

        
        
        

        struct {
            BUS_QUERY_ID_TYPE IdType;
        } QueryId;

        
        
        

        struct {
            DEVICE_TEXT_TYPE DeviceTextType;
            LCID __declspec(align(8)) LocaleId;
        } QueryDeviceText;

        
        
        

        struct {
            BOOLEAN InPath;
            BOOLEAN Reserved[3];
            DEVICE_USAGE_NOTIFICATION_TYPE __declspec(align(8)) Type;
        } UsageNotification;

        
        
        

        struct {
            SYSTEM_POWER_STATE PowerState;
        } WaitWake;

        
        
        

        struct {
            PPOWER_SEQUENCE PowerSequence;
        } PowerSequence;

        
        
        


        struct {
            union {
                ULONG SystemContext;
                SYSTEM_POWER_STATE_CONTEXT SystemPowerStateContext;
            };
            POWER_STATE_TYPE __declspec(align(8)) Type;
            POWER_STATE __declspec(align(8)) State;
            POWER_ACTION __declspec(align(8)) ShutdownType;
        } Power;








        
        
        

        struct {
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
        } StartDevice;

        
        
        
        
        

        
        
        

        struct {
            ULONG_PTR ProviderId;
            PVOID DataPath;
            ULONG BufferSize;
            PVOID Buffer;
        } WMI;

        
        
        

        struct {
            PVOID Argument1;
            PVOID Argument2;
            PVOID Argument3;
            PVOID Argument4;
        } Others;

    } Parameters;

    
    
    
    

    PDEVICE_OBJECT DeviceObject;

    
    
    
    

    PFILE_OBJECT FileObject;

    
    
    
    

    PIO_COMPLETION_ROUTINE CompletionRoutine;

    
    
    
    

    PVOID Context;

} IO_STACK_LOCATION, *PIO_STACK_LOCATION;

#pragma pack(pop)

#elif CST_OSVER == MACRO_OSVER_WIN7 // #if CST_OSVER == MACRO_OSVER_LH

typedef short CSHORT;
typedef CCHAR KPROCESSOR_MODE;
typedef UCHAR KIRQL;


typedef enum {
    BusQueryDeviceID = 0,       // <Enumerator>\<Enumerator-specific device id>
    BusQueryHardwareIDs = 1,    // Hardware ids
    BusQueryCompatibleIDs = 2,  // compatible device ids
    BusQueryInstanceID = 3,     // persistent id for this instance of the device
    BusQueryDeviceSerialNumber = 4,   // serial number for this device
    BusQueryContainerID = 5     // unique id of the device's physical container
} BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;

typedef enum _DEVICE_RELATION_TYPE {
    BusRelations,
    EjectionRelations,
    PowerRelations,
    RemovalRelations,
    TargetDeviceRelation,
    SingleBusRelations,
    TransportRelations
} DEVICE_RELATION_TYPE, *PDEVICE_RELATION_TYPE;


typedef enum {
    DeviceTextDescription = 0,            // DeviceDesc property
    DeviceTextLocationInformation = 1     // DeviceLocation property
} DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;


typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE {
    DeviceUsageTypeUndefined,
    DeviceUsageTypePaging,
    DeviceUsageTypeHibernation,
    DeviceUsageTypeDumpFile
} DEVICE_USAGE_NOTIFICATION_TYPE;


typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation         = 1,
    FileFullDirectoryInformation,   // 2
    FileBothDirectoryInformation,   // 3
    FileBasicInformation,           // 4
    FileStandardInformation,        // 5
    FileInternalInformation,        // 6
    FileEaInformation,              // 7
    FileAccessInformation,          // 8
    FileNameInformation,            // 9
    FileRenameInformation,          // 10
    FileLinkInformation,            // 11
    FileNamesInformation,           // 12
    FileDispositionInformation,     // 13
    FilePositionInformation,        // 14
    FileFullEaInformation,          // 15
    FileModeInformation,            // 16
    FileAlignmentInformation,       // 17
    FileAllInformation,             // 18
    FileAllocationInformation,      // 19
    FileEndOfFileInformation,       // 20
    FileAlternateNameInformation,   // 21
    FileStreamInformation,          // 22
    FilePipeInformation,            // 23
    FilePipeLocalInformation,       // 24
    FilePipeRemoteInformation,      // 25
    FileMailslotQueryInformation,   // 26
    FileMailslotSetInformation,     // 27
    FileCompressionInformation,     // 28
    FileObjectIdInformation,        // 29
    FileCompletionInformation,      // 30
    FileMoveClusterInformation,     // 31
    FileQuotaInformation,           // 32
    FileReparsePointInformation,    // 33
    FileNetworkOpenInformation,     // 34
    FileAttributeTagInformation,    // 35
    FileTrackingInformation,        // 36
    FileIdBothDirectoryInformation, // 37
    FileIdFullDirectoryInformation, // 38
    FileValidDataLengthInformation, // 39
    FileShortNameInformation,       // 40
    FileIoCompletionNotificationInformation, // 41
    FileIoStatusBlockRangeInformation,       // 42
    FileIoPriorityHintInformation,           // 43
    FileSfioReserveInformation,              // 44
    FileSfioVolumeInformation,               // 45
    FileHardLinkInformation,                 // 46
    FileProcessIdsUsingFileInformation,      // 47
    FileNormalizedNameInformation,           // 48
    FileNetworkPhysicalNameInformation,      // 49
    FileIdGlobalTxDirectoryInformation,      // 50
    FileIsRemoteDeviceInformation,           // 51
    FileAttributeCacheInformation,           // 52
    FileNumaNodeInformation,                 // 53
    FileStandardLinkInformation,             // 54
    FileRemoteProtocolInformation,           // 55
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;


typedef enum _FSINFOCLASS {
    FileFsVolumeInformation       = 1,
    FileFsLabelInformation,      // 2
    FileFsSizeInformation,       // 3
    FileFsDeviceInformation,     // 4
    FileFsAttributeInformation,  // 5
    FileFsControlInformation,    // 6
    FileFsFullSizeInformation,   // 7
    FileFsObjectIdInformation,   // 8
    FileFsDriverPathInformation, // 9
    FileFsVolumeFlagsInformation,// 10
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;



typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    Vmcs,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;


typedef enum _IO_ALLOCATION_ACTION {
    KeepObject = 1,
    DeallocateObject,
    DeallocateObjectKeepRegisters
} IO_ALLOCATION_ACTION, *PIO_ALLOCATION_ACTION;


typedef enum _IRQ_DEVICE_POLICY {
    IrqPolicyMachineDefault = 0,
    IrqPolicyAllCloseProcessors,
    IrqPolicyOneCloseProcessor,
    IrqPolicyAllProcessorsInMachine,
    IrqPolicySpecifiedProcessors,
    IrqPolicySpreadMessagesAcrossAllProcessors
} IRQ_DEVICE_POLICY, *PIRQ_DEVICE_POLICY;


typedef enum _IRQ_PRIORITY {
    IrqPriorityUndefined = 0,
    IrqPriorityLow,
    IrqPriorityNormal,
    IrqPriorityHigh
} IRQ_PRIORITY, *PIRQ_PRIORITY;



typedef enum _POWER_STATE_TYPE {
    SystemPowerState = 0,
    DevicePowerState
} POWER_STATE_TYPE, *PPOWER_STATE_TYPE;

typedef struct _SECURITY_SUBJECT_CONTEXT {
    PACCESS_TOKEN ClientToken;
    SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
    PACCESS_TOKEN PrimaryToken;
    PVOID ProcessAuditId;
    } SECURITY_SUBJECT_CONTEXT, *PSECURITY_SUBJECT_CONTEXT;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;



     PWCH   Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;


typedef struct _INITIAL_PRIVILEGE_SET {
    ULONG PrivilegeCount;
    ULONG Control;
    LUID_AND_ATTRIBUTES Privilege[3];
    } INITIAL_PRIVILEGE_SET, * PINITIAL_PRIVILEGE_SET;



typedef struct _ACCESS_STATE {
   LUID OperationID;                // Currently unused, replaced by TransactionId in AUX_ACCESS_DATA
   BOOLEAN SecurityEvaluated;
   BOOLEAN GenerateAudit;
   BOOLEAN GenerateOnClose;
   BOOLEAN PrivilegesAllocated;
   ULONG Flags;
   ACCESS_MASK RemainingDesiredAccess;
   ACCESS_MASK PreviouslyGrantedAccess;
   ACCESS_MASK OriginalDesiredAccess;
   SECURITY_SUBJECT_CONTEXT SubjectSecurityContext;
   PSECURITY_DESCRIPTOR SecurityDescriptor; // it stores SD supplied by caller when creating a new object.
   PVOID AuxData;
   union {
      INITIAL_PRIVILEGE_SET InitialPrivilegeSet;
      PRIVILEGE_SET PrivilegeSet;
      } Privileges;

   BOOLEAN AuditPrivileges;
   UNICODE_STRING ObjectName;
   UNICODE_STRING ObjectTypeName;

   } ACCESS_STATE, *PACCESS_STATE;


typedef struct _IO_SECURITY_CONTEXT {
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_STATE AccessState;
    ACCESS_MASK DesiredAccess;
    ULONG FullCreateOptions;
} IO_SECURITY_CONTEXT, *PIO_SECURITY_CONTEXT;

typedef struct _IO_TIMER *PIO_TIMER;


typedef struct _VPB {
    CSHORT Type;
    CSHORT Size;
    USHORT Flags;
    USHORT VolumeLabelLength; // in bytes
    struct _DEVICE_OBJECT *DeviceObject;
    struct _DEVICE_OBJECT *RealDevice;
    ULONG SerialNumber;
    ULONG ReferenceCount;
    WCHAR VolumeLabel[(32 * sizeof(WCHAR)) / sizeof(WCHAR)];
} VPB, *PVPB;


typedef struct _KDEVICE_QUEUE_ENTRY {
    LIST_ENTRY DeviceListEntry;
    ULONG SortKey;
    BOOLEAN Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY, *PRKDEVICE_QUEUE_ENTRY;

typedef
IO_ALLOCATION_ACTION
DRIVER_CONTROL (
     struct _DEVICE_OBJECT *DeviceObject,
     struct _IRP *Irp,
     PVOID MapRegisterBase,
     PVOID Context
    );
typedef DRIVER_CONTROL *PDRIVER_CONTROL;

 
typedef
void
KDEFERRED_ROUTINE (
     struct _KDPC *Dpc,
     PVOID DeferredContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );

typedef KDEFERRED_ROUTINE *PKDEFERRED_ROUTINE;


typedef struct _KDPC {
    UCHAR Type;
    UCHAR Importance;
    volatile USHORT Number;
    LIST_ENTRY DpcListEntry;
    PKDEFERRED_ROUTINE DeferredRoutine;
    PVOID DeferredContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
     PVOID DpcData;
} KDPC, *PKDPC, *PRKDPC;

typedef struct _WAIT_CONTEXT_BLOCK {
    KDEVICE_QUEUE_ENTRY WaitQueueEntry;
    PDRIVER_CONTROL DeviceRoutine;
    PVOID DeviceContext;
    ULONG NumberOfMapRegisters;
    PVOID DeviceObject;
    PVOID CurrentIrp;
    PKDPC BufferChainingDpc;
} WAIT_CONTEXT_BLOCK, *PWAIT_CONTEXT_BLOCK;


typedef struct _KDEVICE_QUEUE {
    CSHORT Type;
    CSHORT Size;
    LIST_ENTRY DeviceListHead;
    KSPIN_LOCK Lock;



    union {
        BOOLEAN Busy;
        struct {
            LONG64 Reserved : 8;
            LONG64 Hint : 56;
        };
    };
} KDEVICE_QUEUE, *PKDEVICE_QUEUE, *PRKDEVICE_QUEUE;


typedef struct _DISPATCHER_HEADER {
    union {
        struct {
            UCHAR Type;                 // All (accessible via KOBJECT_TYPE)

            union {
                union {                 // Timer
                    UCHAR TimerControlFlags;
                    struct {
                        UCHAR Absolute              : 1;
                        UCHAR Coalescable           : 1;
                        UCHAR KeepShifting          : 1;    // Periodic timer
                        UCHAR EncodedTolerableDelay : 5;    // Periodic timer
                    } ;
                } ;

                UCHAR Abandoned;        // Queue
                BOOLEAN Signalling;     // Gate/Events
            } ;

            union {
                union {
                    UCHAR ThreadControlFlags;  // Thread
                    struct {
                        UCHAR CpuThrottled      : 1;
                        UCHAR CycleProfiling    : 1;
                        UCHAR CounterProfiling  : 1;
                        UCHAR Reserved          : 5;
                    } ;
                } ;
                UCHAR Hand;             // Timer
                UCHAR Size;             // All other objects
            } ;

            union {
                union {                 // Timer
                    UCHAR TimerMiscFlags;
                    struct {



                        UCHAR Index             : 6;







                        UCHAR Inserted          : 1;
                        volatile UCHAR Expired  : 1;
                    } ;
                } ;
                union {                 // Thread
                    BOOLEAN DebugActive;
                    struct {
                        BOOLEAN ActiveDR7       : 1;
                        BOOLEAN Instrumented    : 1;
                        BOOLEAN Reserved2       : 4;
                        BOOLEAN UmsScheduled    : 1;
                        BOOLEAN UmsPrimary      : 1;
                    } ;
                } ;
                BOOLEAN DpcActive;      // Mutant
            } ;
        } ;

        volatile LONG Lock;             // Interlocked
    } ;

    LONG SignalState;                   // Object lock
    LIST_ENTRY WaitListHead;            // Object lock
} DISPATCHER_HEADER;


typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT, *PRKEVENT;


typedef struct __declspec(align(16)) _DEVICE_OBJECT {
    CSHORT Type;
    USHORT Size;
    LONG ReferenceCount;
    struct _DRIVER_OBJECT *DriverObject;
    struct _DEVICE_OBJECT *NextDevice;
    struct _DEVICE_OBJECT *AttachedDevice;
    struct _IRP *CurrentIrp;
    PIO_TIMER Timer;
    ULONG Flags;                                // See above:  DO_...
    ULONG Characteristics;                      // See ntioapi:  FILE_...
     PVPB Vpb;
    PVOID DeviceExtension;
    ULONG DeviceType;
    CCHAR StackSize;
    union {
        LIST_ENTRY ListEntry;
        WAIT_CONTEXT_BLOCK Wcb;
    } Queue;
    ULONG AlignmentRequirement;
    KDEVICE_QUEUE DeviceQueue;
    KDPC Dpc;

    //
    //  The following field is for exclusive use by the filesystem to keep
    //  track of the number of Fsp threads currently using the device
    //

    ULONG ActiveThreadCount;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    KEVENT DeviceLock;

    USHORT SectorSize;
    USHORT Spare1;

    struct _DEVOBJ_EXTENSION  *DeviceObjectExtension;
    PVOID  Reserved;

} DEVICE_OBJECT;

typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT; 


typedef struct _SECTION_OBJECT_POINTERS {
    PVOID DataSectionObject;
    PVOID SharedCacheMap;
    PVOID ImageSectionObject;
} SECTION_OBJECT_POINTERS;
typedef SECTION_OBJECT_POINTERS *PSECTION_OBJECT_POINTERS;


typedef struct _IO_COMPLETION_CONTEXT {
    PVOID Port;
    PVOID Key;
} IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;


typedef struct _FILE_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    PVPB Vpb;
    PVOID FsContext;
    PVOID FsContext2;
    PSECTION_OBJECT_POINTERS SectionObjectPointer;
    PVOID PrivateCacheMap;
    NTSTATUS FinalStatus;
    struct _FILE_OBJECT *RelatedFileObject;
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    ULONG Flags;
    UNICODE_STRING FileName;
    LARGE_INTEGER CurrentByteOffset;
     ULONG Waiters;
     ULONG Busy;
    PVOID LastLock;
    KEVENT Lock;
    KEVENT Event;
     PIO_COMPLETION_CONTEXT CompletionContext;
    KSPIN_LOCK IrpListLock;
    LIST_ENTRY IrpList;
     PVOID FileObjectExtension;
} FILE_OBJECT;
typedef struct _FILE_OBJECT *PFILE_OBJECT; 


typedef struct _FILE_GET_QUOTA_INFORMATION *PFILE_GET_QUOTA_INFORMATION;

typedef void (*PINTERFACE_REFERENCE)(PVOID Context);
typedef void (*PINTERFACE_DEREFERENCE)(PVOID Context);

#undef INTERFACE

typedef struct _INTERFACE {
    USHORT Size;
    USHORT Version;
    PVOID Context;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;
    // interface specific entries go here
} INTERFACE, *PINTERFACE;


typedef  struct _DEVICE_CAPABILITIES {
    USHORT Size;
    USHORT Version;  // the version documented here is version 1
    ULONG DeviceD1:1;
    ULONG DeviceD2:1;
    ULONG LockSupported:1;
    ULONG EjectSupported:1; // Ejectable in S0
    ULONG Removable:1;
    ULONG DockDevice:1;
    ULONG UniqueID:1;
    ULONG SilentInstall:1;
    ULONG RawDeviceOK:1;
    ULONG SurpriseRemovalOK:1;
    ULONG WakeFromD0:1;
    ULONG WakeFromD1:1;
    ULONG WakeFromD2:1;
    ULONG WakeFromD3:1;
    ULONG HardwareDisabled:1;
    ULONG NonDynamic:1;
    ULONG WarmEjectSupported:1;
    ULONG NoDisplayInUI:1;
    ULONG Reserved1:1;
    ULONG Reserved:13;

    ULONG Address;
    ULONG UINumber;

    DEVICE_POWER_STATE DeviceState[7];
    SYSTEM_POWER_STATE SystemWake;
    DEVICE_POWER_STATE DeviceWake;
    ULONG D1Latency;
    ULONG D2Latency;
    ULONG D3Latency;
} DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

typedef struct _IO_RESOURCE_DESCRIPTOR {
    UCHAR Option;
    UCHAR Type;                         // use CM_RESOURCE_TYPE
    UCHAR ShareDisposition;             // use CM_SHARE_DISPOSITION
    UCHAR Spare1;
    USHORT Flags;                       // use CM resource flag defines
    USHORT Spare2;                      // align

    union {
        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Port;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory;

        struct {
            ULONG MinimumVector;
            ULONG MaximumVector;



            IRQ_DEVICE_POLICY AffinityPolicy;
            IRQ_PRIORITY PriorityPolicy;
            KAFFINITY TargetedProcessors;
        } Interrupt;

        struct {
            ULONG MinimumChannel;
            ULONG MaximumChannel;
        } Dma;

        struct {
            ULONG Length;
            ULONG Alignment;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Generic;

        struct {
            ULONG Data[3];
        } DevicePrivate;

        //
        // Bus Number information.
        //

        struct {
            ULONG Length;
            ULONG MinBusNumber;
            ULONG MaxBusNumber;
            ULONG Reserved;
        } BusNumber;

        struct {
            ULONG Priority;   // use LCPRI_Xxx values in cfg.h
            ULONG Reserved1;
            ULONG Reserved2;
        } ConfigData;

        //
        // The following structures provide descriptions
        // for memory resource requirement greater than MAXULONG
        //

        struct {
            ULONG Length40;
            ULONG Alignment40;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory40;

        struct {
            ULONG Length48;
            ULONG Alignment48;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory48;

        struct {
            ULONG Length64;
            ULONG Alignment64;
            PHYSICAL_ADDRESS MinimumAddress;
            PHYSICAL_ADDRESS MaximumAddress;
        } Memory64;


    } u;

} IO_RESOURCE_DESCRIPTOR, *PIO_RESOURCE_DESCRIPTOR;

typedef struct _IO_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;

    ULONG Count;
    IO_RESOURCE_DESCRIPTOR Descriptors[1];
} IO_RESOURCE_LIST, *PIO_RESOURCE_LIST;


typedef struct _IO_RESOURCE_REQUIREMENTS_LIST {
    ULONG ListSize;
    INTERFACE_TYPE InterfaceType; // unused for WDM
    ULONG BusNumber; // unused for WDM
    ULONG SlotNumber;
    ULONG Reserved[3];
    ULONG AlternativeLists;
    IO_RESOURCE_LIST  List[1];
} IO_RESOURCE_REQUIREMENTS_LIST, *PIO_RESOURCE_REQUIREMENTS_LIST;


typedef struct _POWER_SEQUENCE {
    ULONG SequenceD1;
    ULONG SequenceD2;
    ULONG SequenceD3;
} POWER_SEQUENCE, *PPOWER_SEQUENCE;


typedef struct _SYSTEM_POWER_STATE_CONTEXT {
    union {
        struct {
            ULONG   Reserved1             : 8;
            ULONG   TargetSystemState     : 4;
            ULONG   EffectiveSystemState  : 4;
            ULONG   CurrentSystemState    : 4;
            ULONG   IgnoreHibernationPath : 1;
            ULONG   PseudoTransition      : 1;
            ULONG   Reserved2             : 10;
        } ;

        ULONG ContextAsUlong;
    } ;
} SYSTEM_POWER_STATE_CONTEXT, *PSYSTEM_POWER_STATE_CONTEXT;


typedef union _POWER_STATE {
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;

#pragma pack(push,4)

typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        //
        // Range of resources, inclusive.  These are physical, bus relative.
        // It is known that Port and Memory below have the exact same layout
        // as Generic.
        //

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Generic;

        //
        //

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        //
        //

        struct {



            ULONG Level;
            ULONG Vector;
            KAFFINITY Affinity;
        } Interrupt;

        //
        // Values for message signaled interrupts are distinct in the
        // raw and translated cases.
        //

        struct {
            union {
               struct {


                   USHORT Reserved;
                   USHORT MessageCount;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Raw;

               struct {



                   ULONG Level;
                   ULONG Vector;
                   KAFFINITY Affinity;
               } Translated;
            } ;
        } MessageInterrupt;

        //
        // Range of memory addresses, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        //

        struct {
            PHYSICAL_ADDRESS Start;    // 64 bit physical addresses.
            ULONG Length;
        } Memory;

        //
        // Physical DMA channel.
        //

        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        //
        // Device driver private data, usually used to help it figure
        // what the resource assignments decisions that were made.
        //

        struct {
            ULONG Data[3];
        } DevicePrivate;

        //
        // Bus Number information.
        //

        struct {
            ULONG Start;
            ULONG Length;
            ULONG Reserved;
        } BusNumber;

        //
        // Device Specific information defined by the driver.
        // The DataSize field indicates the size of the data in bytes. The
        // data is located immediately after the DeviceSpecificData field in
        // the structure.
        //

        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;

        // The following structures provide support for memory-mapped
        // IO resources greater than MAXULONG
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length40;
        } Memory40;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length48;
        } Memory48;

        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length64;
        } Memory64;


    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;

#pragma pack(pop)

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;


typedef struct _CM_FULL_RESOURCE_DESCRIPTOR {
    INTERFACE_TYPE InterfaceType; // unused for WDM
    ULONG BusNumber; // unused for WDM
    CM_PARTIAL_RESOURCE_LIST PartialResourceList;
} CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;


typedef struct _CM_RESOURCE_LIST {
    ULONG Count;
    CM_FULL_RESOURCE_DESCRIPTOR List[1];
} CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;

typedef  struct _MDL {
    struct _MDL *Next;
    CSHORT Size;
    CSHORT MdlFlags;
    struct _EPROCESS *Process;
    PVOID MappedSystemVa;
    PVOID StartVa;
    ULONG ByteCount;
    ULONG ByteOffset;
} MDL, *PMDL;


typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    } ;

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef
void
(__stdcall *PIO_APC_ROUTINE) (
     PVOID ApcContext,
     PIO_STATUS_BLOCK IoStatusBlock,
     ULONG Reserved
    );

typedef
void
DRIVER_CANCEL (
     struct _DEVICE_OBJECT *DeviceObject,
       struct _IRP *Irp
    );

typedef DRIVER_CANCEL *PDRIVER_CANCEL;

typedef struct _ETHREAD *PETHREAD;

typedef
void
KNORMAL_ROUTINE (
     PVOID NormalContext,
     PVOID SystemArgument1,
     PVOID SystemArgument2
    );
typedef KNORMAL_ROUTINE *PKNORMAL_ROUTINE;

typedef
void
KKERNEL_ROUTINE (
     struct _KAPC *Apc,
     PKNORMAL_ROUTINE *NormalRoutine,
     PVOID *NormalContext,
     PVOID *SystemArgument1,
     PVOID *SystemArgument2
    );
typedef KKERNEL_ROUTINE *PKKERNEL_ROUTINE;

typedef
void
KRUNDOWN_ROUTINE (
     struct _KAPC *Apc
    );
typedef KRUNDOWN_ROUTINE *PKRUNDOWN_ROUTINE;


typedef struct _KAPC {
    UCHAR Type;
    UCHAR SpareByte0;
    UCHAR Size;
    UCHAR SpareByte1;
    ULONG SpareLong0;
    struct _KTHREAD *Thread;
    LIST_ENTRY ApcListEntry;
    PKKERNEL_ROUTINE KernelRoutine;
    PKRUNDOWN_ROUTINE RundownRoutine;
    PKNORMAL_ROUTINE NormalRoutine;
    PVOID NormalContext;

    //
    // N.B. The following two members MUST be together.
    //

    PVOID SystemArgument1;
    PVOID SystemArgument2;
    CCHAR ApcStateIndex;
    KPROCESSOR_MODE ApcMode;
    BOOLEAN Inserted;
} KAPC, *PKAPC, *PRKAPC;

typedef struct __declspec(align(16)) _IRP {
    CSHORT Type;
    USHORT Size;

    //
    // Define the common fields used to control the IRP.
    //

    //
    // Define a pointer to the Memory Descriptor List (MDL) for this I/O
    // request.  This field is only used if the I/O is "direct I/O".
    //

    PMDL MdlAddress;

    //
    // Flags word - used to remember various flags.
    //

    ULONG Flags;

    //
    // The following union is used for one of three purposes:
    //
    //    1. This IRP is an associated IRP.  The field is a pointer to a master
    //       IRP.
    //
    //    2. This is the master IRP.  The field is the count of the number of
    //       IRPs which must complete (associated IRPs) before the master can
    //       complete.
    //
    //    3. This operation is being buffered and the field is the address of
    //       the system space buffer.
    //

    union {
        struct _IRP *MasterIrp;
         LONG IrpCount;
        PVOID SystemBuffer;
    } AssociatedIrp;

    //
    // Thread list entry - allows queueing the IRP to the thread pending I/O
    // request packet list.
    //

    LIST_ENTRY ThreadListEntry;

    //
    // I/O status - final status of operation.
    //

    IO_STATUS_BLOCK IoStatus;

    //
    // Requestor mode - mode of the original requestor of this operation.
    //

    KPROCESSOR_MODE RequestorMode;

    //
    // Pending returned - TRUE if pending was initially returned as the
    // status for this packet.
    //

    BOOLEAN PendingReturned;

    //
    // Stack state information.
    //

    CHAR StackCount;
    CHAR CurrentLocation;

    //
    // Cancel - packet has been canceled.
    //

    BOOLEAN Cancel;

    //
    // Cancel Irql - Irql at which the cancel spinlock was acquired.
    //

    KIRQL CancelIrql;

    //
    // ApcEnvironment - Used to save the APC environment at the time that the
    // packet was initialized.
    //

    CCHAR ApcEnvironment;

    //
    // Allocation control flags.
    //

    UCHAR AllocationFlags;

    //
    // User parameters.
    //

    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;
    union {
        struct {
            union {
                PIO_APC_ROUTINE UserApcRoutine;
                PVOID IssuingProcess;
            };
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    //
    // CancelRoutine - Used to contain the address of a cancel routine supplied
    // by a device driver when the IRP is in a cancelable state.
    //

     PDRIVER_CANCEL CancelRoutine;

    //
    // Note that the UserBuffer parameter is outside of the stack so that I/O
    // completion can copy data back into the user's address space without
    // having to know exactly which service was being invoked.  The length
    // of the copy is stored in the second half of the I/O status block. If
    // the UserBuffer field is NULL, then no copy is performed.
    //

    PVOID UserBuffer;

    //
    // Kernel structures
    //
    // The following section contains kernel structures which the IRP needs
    // in order to place various work information in kernel controller system
    // queues.  Because the size and alignment cannot be controlled, they are
    // placed here at the end so they just hang off and do not affect the
    // alignment of other fields in the IRP.
    //

    union {

        struct {

            union {

                //
                // DeviceQueueEntry - The device queue entry field is used to
                // queue the IRP to the device driver device queue.
                //

                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;

                struct {

                    //
                    // The following are available to the driver to use in
                    // whatever manner is desired, while the driver owns the
                    // packet.
                    //

                    PVOID DriverContext[4];

                } ;

            } ;

            //
            // Thread - pointer to caller's Thread Control Block.
            //

            PETHREAD Thread;

            //
            // Auxiliary buffer - pointer to any auxiliary buffer that is
            // required to pass information to a driver that is not contained
            // in a normal buffer.
            //

            PCHAR AuxiliaryBuffer;

            //
            // The following unnamed structure must be exactly identical
            // to the unnamed structure used in the minipacket header used
            // for completion queue entries.
            //

            struct {

                //
                // List entry - used to queue the packet to completion queue, among
                // others.
                //

                LIST_ENTRY ListEntry;

                union {

                    //
                    // Current stack location - contains a pointer to the current
                    // IO_STACK_LOCATION structure in the IRP stack.  This field
                    // should never be directly accessed by drivers.  They should
                    // use the standard functions.
                    //

                    struct _IO_STACK_LOCATION *CurrentStackLocation;

                    //
                    // Minipacket type.
                    //

                    ULONG PacketType;
                };
            };

            //
            // Original file object - pointer to the original file object
            // that was used to open the file.  This field is owned by the
            // I/O system and should not be used by any other drivers.
            //

            PFILE_OBJECT OriginalFileObject;

        } Overlay;

        //
        // APC - This APC control block is used for the special kernel APC as
        // well as for the caller's APC, if one was specified in the original
        // argument list.  If so, then the APC is reused for the normal APC for
        // whatever mode the caller was in and the "special" routine that is
        // invoked before the APC gets control simply deallocates the IRP.
        //

        KAPC Apc;

        //
        // CompletionKey - This is the key that is used to distinguish
        // individual I/O operations initiated on a single file handle.
        //

        PVOID CompletionKey;

    } Tail;

} IRP;

typedef IRP *PIRP;



typedef
NTSTATUS
IO_COMPLETION_ROUTINE (
     PDEVICE_OBJECT DeviceObject,
     PIRP Irp,
     PVOID Context
    );

typedef IO_COMPLETION_ROUTINE *PIO_COMPLETION_ROUTINE;


typedef struct _IO_STACK_LOCATION {
    UCHAR MajorFunction;
    UCHAR MinorFunction;
    UCHAR Flags;
    UCHAR Control;

    //
    // The following user parameters are based on the service that is being
    // invoked.  Drivers and file systems can determine which set to use based
    // on the above major and minor function codes.
    //

    union {

        //
        // System service parameters for:  NtCreateFile
        //

        struct {
            PIO_SECURITY_CONTEXT SecurityContext;
            ULONG Options;
            USHORT __declspec(align(8)) FileAttributes;
            USHORT ShareAccess;
            ULONG __declspec(align(8)) EaLength;
        } Create;


        //
        // System service parameters for:  NtReadFile
        //

        struct {
            ULONG Length;
            ULONG __declspec(align(8)) Key;
            LARGE_INTEGER ByteOffset;
        } Read;

        //
        // System service parameters for:  NtWriteFile
        //

        struct {
            ULONG Length;
            ULONG __declspec(align(8)) Key;
            LARGE_INTEGER ByteOffset;
        } Write;

        //
        // System service parameters for:  NtQueryDirectoryFile
        //

        struct {
            ULONG Length;
            PUNICODE_STRING FileName;
            FILE_INFORMATION_CLASS FileInformationClass;
            ULONG __declspec(align(8)) FileIndex;
        } QueryDirectory;

        //
        // System service parameters for:  NtNotifyChangeDirectoryFile
        //

        struct {
            ULONG Length;
            ULONG __declspec(align(8)) CompletionFilter;
        } NotifyDirectory;

        //
        // System service parameters for:  NtQueryInformationFile
        //

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS __declspec(align(8)) FileInformationClass;
        } QueryFile;

        //
        // System service parameters for:  NtSetInformationFile
        //

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS __declspec(align(8)) FileInformationClass;
            PFILE_OBJECT FileObject;
            union {
                struct {
                    BOOLEAN ReplaceIfExists;
                    BOOLEAN AdvanceOnly;
                };
                ULONG ClusterCount;
                HANDLE DeleteHandle;
            };
        } SetFile;



        //
        // System service parameters for:  NtQueryEaFile
        //

        struct {
            ULONG Length;
            PVOID EaList;
            ULONG EaListLength;
            ULONG __declspec(align(8)) EaIndex;
        } QueryEa;

        //
        // System service parameters for:  NtSetEaFile
        //

        struct {
            ULONG Length;
        } SetEa;



        //
        // System service parameters for:  NtQueryVolumeInformationFile
        //

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS __declspec(align(8)) FsInformationClass;
        } QueryVolume;



        //
        // System service parameters for:  NtSetVolumeInformationFile
        //

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS __declspec(align(8)) FsInformationClass;
        } SetVolume;

        //
        // System service parameters for:  NtFsControlFile
        //
        // Note that the user's output buffer is stored in the UserBuffer field
        // and the user's input buffer is stored in the SystemBuffer field.
        //

        struct {
            ULONG OutputBufferLength;
            ULONG __declspec(align(8)) InputBufferLength;
            ULONG __declspec(align(8)) FsControlCode;
            PVOID Type3InputBuffer;
        } FileSystemControl;
        //
        // System service parameters for:  NtLockFile/NtUnlockFile
        //

        struct {
            PLARGE_INTEGER Length;
            ULONG __declspec(align(8)) Key;
            LARGE_INTEGER ByteOffset;
        } LockControl;

        //
        // System service parameters for:  NtFlushBuffersFile
        //
        // No extra user-supplied parameters.
        //



        //
        // System service parameters for:  NtCancelIoFile
        //
        // No extra user-supplied parameters.
        //



        //
        // System service parameters for:  NtDeviceIoControlFile
        //
        // Note that the user's output buffer is stored in the UserBuffer field
        // and the user's input buffer is stored in the SystemBuffer field.
        //

        struct {
            ULONG OutputBufferLength;
            ULONG __declspec(align(8)) InputBufferLength;
            ULONG __declspec(align(8)) IoControlCode;
            PVOID Type3InputBuffer;
        } DeviceIoControl;

        //
        // System service parameters for:  NtQuerySecurityObject
        //

        struct {
            SECURITY_INFORMATION SecurityInformation;
            ULONG __declspec(align(8)) Length;
        } QuerySecurity;

        //
        // System service parameters for:  NtSetSecurityObject
        //

        struct {
            SECURITY_INFORMATION SecurityInformation;
            PSECURITY_DESCRIPTOR SecurityDescriptor;
        } SetSecurity;

        //
        // Non-system service parameters.
        //
        // Parameters for MountVolume
        //

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } MountVolume;

        //
        // Parameters for VerifyVolume
        //

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } VerifyVolume;

        //
        // Parameters for Scsi with internal device contorl.
        //

        struct {
            struct _SCSI_REQUEST_BLOCK *Srb;
        } Scsi;



        //
        // System service parameters for:  NtQueryQuotaInformationFile
        //

        struct {
            ULONG Length;
            PSID StartSid;
            PFILE_GET_QUOTA_INFORMATION SidList;
            ULONG SidListLength;
        } QueryQuota;

        //
        // System service parameters for:  NtSetQuotaInformationFile
        //

        struct {
            ULONG Length;
        } SetQuota;



        //
        // Parameters for IRP_MN_QUERY_DEVICE_RELATIONS
        //

        struct {
            DEVICE_RELATION_TYPE Type;
        } QueryDeviceRelations;

        //
        // Parameters for IRP_MN_QUERY_INTERFACE
        //

        struct {
            const GUID *InterfaceType;
            USHORT Size;
            USHORT Version;
            PINTERFACE Interface;
            PVOID InterfaceSpecificData;
        } QueryInterface;

        //
        // Parameters for IRP_MN_QUERY_CAPABILITIES
        //

        struct {
            PDEVICE_CAPABILITIES Capabilities;
        } DeviceCapabilities;

        //
        // Parameters for IRP_MN_FILTER_RESOURCE_REQUIREMENTS
        //

        struct {
            PIO_RESOURCE_REQUIREMENTS_LIST IoResourceRequirementList;
        } FilterResourceRequirements;

        //
        // Parameters for IRP_MN_READ_CONFIG and IRP_MN_WRITE_CONFIG
        //

        struct {
            ULONG WhichSpace;
            PVOID Buffer;
            ULONG Offset;
            ULONG __declspec(align(8)) Length;
        } ReadWriteConfig;

        //
        // Parameters for IRP_MN_SET_LOCK
        //

        struct {
            BOOLEAN Lock;
        } SetLock;

        //
        // Parameters for IRP_MN_QUERY_ID
        //

        struct {
            BUS_QUERY_ID_TYPE IdType;
        } QueryId;

        //
        // Parameters for IRP_MN_QUERY_DEVICE_TEXT
        //

        struct {
            DEVICE_TEXT_TYPE DeviceTextType;
            LCID __declspec(align(8)) LocaleId;
        } QueryDeviceText;

        //
        // Parameters for IRP_MN_DEVICE_USAGE_NOTIFICATION
        //

        struct {
            BOOLEAN InPath;
            BOOLEAN Reserved[3];
            DEVICE_USAGE_NOTIFICATION_TYPE __declspec(align(8)) Type;
        } UsageNotification;

        //
        // Parameters for IRP_MN_WAIT_WAKE
        //

        struct {
            SYSTEM_POWER_STATE PowerState;
        } WaitWake;

        //
        // Parameter for IRP_MN_POWER_SEQUENCE
        //

        struct {
            PPOWER_SEQUENCE PowerSequence;
        } PowerSequence;

        //
        // Parameters for IRP_MN_SET_POWER and IRP_MN_QUERY_POWER
        //


        struct {
            union {
                ULONG SystemContext;
                SYSTEM_POWER_STATE_CONTEXT SystemPowerStateContext;
            };
            POWER_STATE_TYPE __declspec(align(8)) Type;
            POWER_STATE __declspec(align(8)) State;
            POWER_ACTION __declspec(align(8)) ShutdownType;
        } Power;








        //
        // Parameters for StartDevice
        //

        struct {
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
        } StartDevice;

        //
        // Parameters for Cleanup
        //
        // No extra parameters supplied
        //

        //
        // WMI Irps
        //

        struct {
            ULONG_PTR ProviderId;
            PVOID DataPath;
            ULONG BufferSize;
            PVOID Buffer;
        } WMI;

        //
        // Others - driver-specific
        //

        struct {
            PVOID Argument1;
            PVOID Argument2;
            PVOID Argument3;
            PVOID Argument4;
        } Others;

    } Parameters;

    //
    // Save a pointer to this device driver's device object for this request
    // so it can be passed to the completion routine if needed.
    //

    PDEVICE_OBJECT DeviceObject;

    //
    // The following location contains a pointer to the file object for this
    // request.
    //

    PFILE_OBJECT FileObject;

    //
    // The following routine is invoked depending on the flags in the above
    // flags field.
    //

    PIO_COMPLETION_ROUTINE CompletionRoutine;

    //
    // The following is used to store the address of the context parameter
    // that should be passed to the CompletionRoutine.
    //

    PVOID Context;

} IO_STACK_LOCATION, *PIO_STACK_LOCATION;




#endif // #elif CST_OSVER == MACRO_OSVER_WIN7
#endif // #elif ARCHITECTURE == MACRO_ARCH_X64


#if defined(__cplusplus)

} // extern "C"

#endif // #if defined(__cplusplus)

#pragma warning(pop)
