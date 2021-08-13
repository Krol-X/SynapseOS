#include "../include/stdlib.h"


//FAT
struct FAT_Parameter_block {
    uint16 Sector_Size;
    uint16 Sectors_Per_Cluster;
    uint16 Reserved_Sectors;
    uint16 FAT_Count;
    uint16 Root_Entries;
    uint16 Total_Sectors;
    uint16 Media_Descriptor;
    uint16 Sectors_Per_FAT;
    uint16 Sectors_Per_Track;
    uint16 Heads;
    uint16 Hidden_sectors;
    uint16 Big_Total_Sectors;
};


struct FAT_Directory_entry {
    char Name[8];
    char Extension[3];
    uint16 File_Attribute;
    char Reserved[10];
    uint16 Time;
    uint16 Date;
    uint16 Cluster_No;
    uint32 Size;
};


struct ext2_super_block {
    uint32 s_inodes_count;
    uint32 s_blocks_count;
    uint32 s_r_blocks_count;
    uint32 s_free_blocks_count;
    uint32 s_free_inodes_count;
    uint32 s_first_data_block;
    uint32 s_log_block_size;
    int32 s_log_frag_size;
    uint32 s_blocks_per_group;
    uint32 s_frags_per_group;
    uint32 s_inodes_per_group;
    uint32 s_mtime;
    uint32 s_wtime;
    uint16 s_mnt_count;
    uint16 s_max_mnt_count;
    uint16 s_magic;
    uint16 s_state;
    uint16 s_errors;
    uint16 s_pad;
    uint32 s_lastcheck;
    uint32 s_checkinterval;
    uint32 s_reserved[238];
};