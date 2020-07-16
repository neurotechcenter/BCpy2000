#ifndef _VHT_NODE_TYPES_H_
#define _VHT_NODE_TYPES_H_

/*------------------------------------------------------------------------*/
typedef enum {
    VHT_LEAF            = 0x10000000,
    VHT_GROUP           = 0x20000000,
    VHT_NODECOMPONENT   = 0x40000000,
    VHT_SHAPE3D         = VHT_LEAF           | 0x0001,
    VHT_BEHAVIOR        = VHT_LEAF           | 0x0002,
    VHT_TRANSFORMGROUP  = VHT_GROUP          | 0x0004,
    VHT_SWITCH          = VHT_GROUP          | 0x0008,
    VHT_COMPONENT       = VHT_TRANSFORMGROUP | 0x0010,
    VHT_HUMAN_HAND      = VHT_COMPONENT      | 0x0020,
    VHT_HUMAN_FINGER    = VHT_GROUP          | 0x0040,
    VHT_PHALANX         = VHT_GROUP          | 0x0080,
    VHT_GEOMETRY        = VHT_NODECOMPONENT  | 0x0200,
    VHT_VERTEXGEOMETRY  = VHT_GEOMETRY       | 0x0400,
    VHT_UNKNOWN         = 0x0000
} vhtNodeType;

#endif


