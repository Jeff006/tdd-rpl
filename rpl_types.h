
/**
 * RPL module
 * Based on https://tools.ietf.org/html/rfc6550
 *
 * Definitions:
 * DAG: Direct Acyclic Graph
 * DAG root: node on the graph with no outgoing edge
 * Destination-Oriented DAG (DODAG): DAG rooted at a single destination (ie. DAG root) with no outgoing edges
 *
 * Algorithm Overview:
 * Advertise presence with DIO (multicast)
 * Listen for DIO, use information to join/maintain DODAG
 * Provision routing entries for destinations in DIO message for next hop use
 *
 * Progress:
 * 2015-01-16 - Ryan - Implemented structures and definitions up to page 50 of RFC
 * 2015-02-16 - Ryan - Continuing structures and definitions up to page 54 of RFC
 *
 */

#include <stdint.h>

#define RPL_MAX_INSTANCE_ID                 127     //!< Maximum instance id in a LLN
#define RPL_INSTANCE_FLAG_GLOBAL            0x00    //!< Indicates the RPL instance is global
#define RPL_INSTANCE_FLAG_LOCAL             0x80    //!< Indicates the RPL instance is local only
#define RPL_INSTANCE_FLAG_LOCAL_DIRECTION   0x40    //!< Direction flag for local RPL instances, 0 in rpl control messages, 1 is towards DODAGID 0 is from DODAGID for data packets

#define RPL_ICMPV6_INFORMATION_TYPE         155     //!< ICMPv6 information message with type of 155 (may be) used for RPL control

#define MAX_OPTION_DATA                     64      //!< Maximum size of control message option data fields

#ifndef DEFAULT_PATH_CONTROL_SIZE
//TODO: Default here
#endif

#ifndef DEFAULT_DIO_INTERVAL_DOUBLINGS
//TODO: Default here
#endif

#ifndef DEFAULT_DIO_INTERVAL_MIN
//TODO: Default here
#endif

#ifndef DEFAULT_DIO_REDUNDANCY_CONSTANT
//TODO: Default here
#endif

#ifndef DEFAULT_MIN_HOP_RANK_INCREASE
//TODO: Default here
#endif

enum rpl_control_message_e {
    RPL_DODAG_INFORMATION_SOLICITATION = 0x00,
    RPL_DODAG_INFORMATION_OBJECT = 0x01,
    RPL_DESTINATION_ADVERTISEMENt_OBJECT = 0x02,
    RPL_DESTINATION_ADVERTISEMENT_OBJECT_ACK = 0x03,
    RPL_SECURE_DODAG_INFORMATION_SOLICITATION = 0x80,
    RPL_SECURE_DODAG_INFORMATION_OBJECT = 0x81,
    RPL_SECURE_DESTINATION_ADVERTISEMENt_OBJECT = 0x82,
    RPL_SECURE_DESTINATION_ADVERTISEMENT_OBJECT_ACK = 0x83,
    RPL_CONSISTENCY_CHECK = 0x8A
};

#ifndef RPL_OVERRIDE_TYPES
typedef uint8_t rpl_instance_t;
typedef uint8_t rpl_dodag_id_t;
typedef uint8_t rpl_dodag_version_t;
typedef uint16_t rpl_dodag_rank_t;
#endif

/**
 * @brief RPL instance structure
 * @details [long description]
 *
 * @param is [description]
 * @return [description]
 */
struct rpl_instance_s {
    rpl_dodag_id_t dodag_id;                //!< Identified of the DODAG root, unique within an RPL instance.
    rpl_dodag_version_t dodag_version;      //!< Specific iteration of a DODAG with a given DODAG ID, sequential counter incremented by the root
    rpl_dodag_rank_t dodag_rank;            //!< Rank in the DODAG (scope is current DODAG version). Defines position wrt. DODAG root.
} rpl_instance_s;

/**
 * @brief DODAG Information Object (DIO)
 * @details [long description]
 *
 */
struct rpl_dio_s {
    rpl_instance_t rpl_instance_id;         //!< RPL instance ID set by DODAG root to indicate which RPL instance the DODAG is part of.
    rpl_dodag_version_t rpl_version;        //!< DODAG Version Number set by DODAG root.
    rpl_dodag_rank_t rank;                  //!< DODAG rank of the node sending the DIO message
    uint8_t mode;                           //!< Mode flags. Includes Grounded, Mode of Operation (MOP), and DODAG Preference (PRE).
    uint8_t dtsn;                           //!< Destination Advertisement Trigger Sequence Number. Set by node issuing DIO, used to maintain Downward routes.
    uint8_t flags;                          //!< Reserved for flags. Must be initialized to zero and ignored by receiver.
    uint8_t reserved;                       //!< Unused field. Must be initialized to zero and ignored by receiver.
    uint8_t options;
} rpl_dio_s;

#define RPL_DIO_MODE_GROUNDED_FLAG              0x80        //!< Indicates whether the DODAG advertised can satisfy the application defined goal.

#define RPL_DIO_MODE_MODE_OF_OPERATION_MASK     0x38        //!< Mask for Mode of Operation setting in DIO mode field (0B00111000)
#define RPL_DIO_MODE_MODE_OF_OPERATION_SHIFT    3           //!< Shift for Mode of Operation setting in DIO mode field

/**
 * Sets the mode of operation for the DIO
 * Used in the rpl_dio_s structure in the mode field with the appropriate shift
 */
enum rpl_dio_mode_mode_of_operation_e {
    RPL_DIO_MODE_MOP0 = 0x00,               //!< No downwards routes maintained by RPL
    RPL_DIO_MODE_MOP1 = 0x01,               //!< Non-Storing mode of operation
    RPL_DIO_MODE_MOP2 = 0x02,               //!< Storing mode of operation without multicast support
    RPL_DIO_MODE_MOP3 = 0x03,               //!< Storing mode of operation with multicast support
};

#define RPL_DIO_MODE_PREFERENCE_MAX         7           //!< Indicates how preferable the root of this DODAG is compared to other DODAG roots in the instance
#define RPL_DIO_MODE_PREFERENCE_DEFAULT     0           //!< Default preference of 0 (least preferred)
#define RPL_DIO_MODE_PREFERENCE_MASK        0x07        //!< Mask for DODAG Preference setting in DIO mode field (0B00000111)
#define RPL_DIO_MODE_PREFERENCE_SHIFT       0           //!< Shift for DODAG Preference setting in DIO mode field

/**
 * Options for RPL DIO packets
 */
enum rpl_dio_options_s {
    RPL_DIO_OPTION_PAD1 = 0x00,
    RPL_DIO_OPTION_PADN = 0x01,
    RPL_DIO_OPTION_DAG_METRIC_CONTAINER = 0x02,
    RPL_DIO_OPTION_ROUTING_INFO = 0x03,
    RPL_DIO_OPTION_DODAG_CONFIG = 0x04,
    RPL_DIO_OPTION_PREFIX_INFO = 0x08
};

/**
 * @brief Destination Advertisement Object (DAO)
 * @details DAO messages are used to establish downwards routes in the DODAG
 * This is optional to support point-to-multipoint (P2MP) and point-to-point (P2P) traffic
 *
 */
struct rpl_dao_s {
    rpl_instance_t rpl_instance;            //!< RPL instance ID set by DODAG root to indicate which RPL instance the DODAG is part of.
    uint8_t flags;                          //!< 'K' flag indicating ACK is required, 'D' flag indicating DODAGID field is present, further bits are reserved.
    uint8_t reserved;                       //!< Reserved for flags. Must be initialized to zero and ignored by receiver.
    uint8_t dao_sequence;                   //!< Incremented at each unique DAO message from a node and echoed in the DAO-ACK message.
    uint8_t dodag_id[16];                   //!< (Optional) set by DODAG root to uniquely identify a DODAG, present only when 'D' flag is set.
    uint8_t options;
} rpl_dao_s;

#define RPL_DAO_FLAGS_MASK                  0x3F    //!< Mask for unused flags in the dao flags field
#define RPL_DAO_FLAG_K_MASK                 0x80    //!< Indicates the recipient must respond with a DAO-ACK
#define RPL_DAO_FLAG_D_MASK                 0x40    //!< Indicates the DODAGID field is present, this MUST be set when a local RPL instance ID is used

/**
 * Options available for rpl dao message
 */
enum rpl_dao_options_e {
    RPL_DAO_OPTION_PAD1 = 0x00,
    RPL_DAO_OPTION_PADN = 0x01,
    RPL_DAO_OPTION_RPL_TARGET = 0x05,
    RPL_DAO_OPTION_TRANSIT_INFORMATION = 0x06,
    RPL_DAO_OPTION_RPL_TARGET_DESCRIPTOR = 0x09
};

/**
 * @brief DODAG Information Solicitation (DIS)
 * @details [long description]
 *
 */
struct rpl_dis_s {
    uint8_t flags;      //!< Unused field reserved for flags.
    uint8_t reserved;   //!< Unused field. Must be initialized to zero and ignored by the receiver.
    uint8_t options;      //!< Options placeholder
} rpl_dis_s;

enum rpl_dis_option_e {
    RPL_DIS_OPTION_PAD1 = 0x00,
    RPL_DIS_OPTION_PADN = 0x01,
    RPL_DIS_OPTION_SOLICITED = 0x07
};

struct rpl_dao_ack_s {
    rpl_instance_t rpl_instance;            //!< RPL instance ID set by DODAG root to indicate which RPL instance the DODAG is part of.
    uint8_t flags;                          //!< 'D' flag indicating DODAGID field is present, further bits are reserved.
    uint8_t dao_sequence;                   //!< Incremented at each unique DAO message from a node and echoed in the DAO-ACK message by the recipient.
    uint8_t status;                         //!< Indicates the completion. Status 0 is unqualified acceptance, 1-127 tentative acceptance, 128-255 rejection.
    uint8_t dodag_id[16];                   //!< (Optional) set by DODAG root to uniquely identify a DODAG, present only when 'D' flag is set.
} rpl_dao_ack_s;

#define RPL_DAO_ACK_FLAGS_MASK              0x7F    //!< Mask for unused flags in the dao ack flags field
#define RPL_DAO_ACK_FLAG_K_MASK             0x80    //!< Indicates the DODAGID field is present, this MUST be set when a local RPL instance ID is used

enum rpl_dao_ack_status_e {
    rpl_dao_ack_status_accepted = 0x00      //!< Indicates the DAO message has been accepted without qualification.
};

#define RPL_DAO_ACK_STATUS_ACCEPTED(status)     (status == 0)
#define RPL_DAO_ACK_STATUS_TENTATIVE(status)    ((status > 0) && (status <= 127))
#define RPL_DAO_ACK_STATUS_REJECTED(status)     ((status > 127) && (status <= 255))

/**
 * @brief Consistency Check (CC)
 * @details The CC message is used to check secure message counters and issue
 * challenge-responses.  A CC message MUST be sent as a secured RPL
 * message.
 */
struct rpl_cc_s {
    rpl_instance_t rpl_instance;            //!< RPL instance ID set by DODAG root to indicate which RPL instance the DODAG is part of.
    uint8_t flags;                          //!< 'R' flag indicating whether CC is a response. 0 for request, 1 for response. Further bits are reserved.
    uint16_t cc_nonce;                      //!< Identifier for the consistency check packet. Corresponding response must have the same nonce as the request.
    uint8_t dodag_id[16];                   //!< Set by DODAG root to uniquely identify a DODAG, present only when 'D' flag is set.
    uint32_t destination_counter;           //!< Indicates the senders estimate of the destinations current security counter value. 0 for no estimate.
    uint8_t opions;
} rpl_cc_s;

#define RPL_CC_FLAGS_MASK                   0x7F    //!< Mask for unused flags in the CC flags field

enum rpl_cc_option_e {
    RPL_CC_OPTION_PAD1 = 0x00,
    RPL_CC_OPTION_PADN = 0x01,
};

/***            RPL Generic Option Structures, flags and enumerations      ***/

/**
 * Option type enumerations
 *
 */
enum rpl_option_type_e {
    RPL_OPTION_PAD1 = 0x00,                 //!< Pad1 Option
    RPL_OPTION_PADN = 0x01,                 //!< PadN Option
    RPL_OPTION_DAG_METRIC = 0x02,           //!< DAG Metric option
    RPL_OPTION_ROUTE_INFO = 0x03,           //!< ROute information option
    RPL_OPTION_DODAG_CONFIGURATION = 0x04,  //!< DODAG Configuration option
    RPL_OPTION_RPL_TARGET = 0x05            //!< RPL target option
};

/**
 * @brief Pad1 Option
 * @details Used to insert a single octet of padding into a message to enable options alignment
 * MAY be present in DIS, DIO, DAO, DAO-ACK, and CC messages
 * nb. The format of the Pad1 option is a special case -- it has neither Option Length nor Option Data fields.
 *
 * Option Type: 0x00
 */
struct rpl_option_pad1_s {
} rpl_option_pad1_s;

/**
 * @brief PadN Option
 * @details The PadN option is used to insert two or more octets of padding into
 * the message to enable options alignment.  PadN option data MUST be
 * ignored by the receiver
 * MAY be present in DIS, DIO, DAO, DAO-ACK, and CC messages
 *
 * Option Type: 0x01
 * Length: For N octets of padding, where 2 <= N <= 7, the Option
 *      Length field contains the value N-2.  An Option Length of 0
 *      indicates a total padding of 2 octets.  An Option Length of 5
 *      indicates a total padding of 7 octets, which is the maximum
 *      padding size allowed with the PadN option.
 * Option Data: For N (N > 1) octets of padding, the Option Data
        consists of N-2 zero-valued octets.
 */
struct rpl_option_padN_s {
    uint8_t padding[];
} rpl_option_padN_s;

/**
 * @brief DAG Metric Container
 * @details The DAG Metric Container is used to report metrics along the DODAG.
 * The DAG Metric Container may contain a number of discrete node, link,
 * and aggregate path metrics and constraints specified in [RFC6551] as
 * chosen by the implementer.
 * MAY be present in DIO or DAO messages
 *
 * Option Type: 0x02
 * Option Length: The Option Length field contains the length in octets
 *       of the Metric Data.
 *
 */
struct rpl_option_dag_metric_s {
    uint8_t metric_data[];
} rpl_option_dag_metric_s;

/**
 * @brief Route Information Option (RIO)
 * @details The RIO is used to indicate that connectivity to the specified
 * destination prefix is available from the DODAG root.

 * In the event that a RPL control message may need to specify
 * connectivity to more than one destination, the RIO may be repeated.
 *
 * Option Type: 0x03
 *
 */
struct rpl_option_route_info_s {
    uint8_t prefix_length;     //!< Number of leading bits in the prefix that are valid (0 to 128)
    uint8_t flags;             //!< Route info flags, contains Route Preference (PRF)
    uint32_t route_lifetime;   //!< ROute lifetime, length of time in seconds that the prefix is valid for route determination
    uint8_t prefix[];          //!< Variable length field containing an IP address or IPv6 prefix
} rpl_option_route_info_s;

#define RPL_OPTION_ROUTE_INFO_PRF_MASK      0x1f        //!< Route preference mask (in flags variable)
#define RPL_OPTION_ROUTE_INGO_PRF_SHIFT     3           //!< Route preference shift (in flags variable)

/**
 * @brief DODAG Configuration option
 * @details The DODAG Configuration option is used to distribute configuration
 * information for DODAG Operation through the DODAG
 * The information communicated in this option is generally static and
 * unchanging within the DODAG, therefore it is not necessary to include
 * in every DIO.
 *
 * Option Type: 0x04
 * Option Length: 14
 */
struct rpl_option_dodag_configuration_s {
    uint8_t flags;                  //!< DODAG configuration flags, contains Authentication Enabled flag and Path Control Size variable
    uint8_t dio_int_double;         //!< DIO Interval doubling, used to configure IMax of the DIO Trickle Timer [rfc6550 section 8.3.1]
    uint8_t dio_int_min;            //!< DIO Interval minimum, used to configure IMin of the DIO Trickle Timer [rfc6550 section 8.3.1]
    uint8_t dio_redun;              //!< DIO Redundancy Constant, used to configure k of the DIO Trickle TImer [rfc6550 section 8.3.1]
    uint16_t max_rank_increase;     //!< Max rank increase, used to configure DAGMaxRankIncrease, 0 to disable
    uint16_t min_hop_rank_increase; //!< Minimum rank increase, used to configure MinHopRankIncrease [rfc6550 section 3.5.1]
    uint16_t objective_code_point;  //!< Identifies the Objective Function (managed by IANA)
    uint8_t reserved;               //!< Reserved field, must be initialized to zero by sender and ignored by receiver
    uint8_t default_lifetime;       //!< Lifetime to be used as default for all RPL routes, lifetime = default * unit
    uint16_t lifetime_unit;         //!< Lifetime unit, provides the unit in seconds used to express route lifetimes in RPL
} rpl_option_dodag_configuration_s;

#define RPL_OPTION_DODAG_CONFIG_AUTHENTICATION_MASK         0x0f        //!< Authentication enabled mask (see flags field)
#define RPL_OPTION_DODAG_CONFIG_AUTHENTICATION_SHIFT        3           //!< Authentication enabled shift
#define RPL_OPTION_DODAG_CONFIG_AUTHENTICATION_ENABLED      (1<<RPL_OPTION_DODAG_CONFIG_AUTHENTICATION_SHIFT)
#define RPL_OPTION_DODAG_CONFIG_AUTHENTICATION_DISABLED     (0<<RPL_OPTION_DODAG_CONFIG_AUTHENTICATION_SHIFT)

#define RPL_OPTION_DODAG_CONFIG_PATH_CONTROL_SIZE_MASK      0x07        //!< Path control size mask (see flags field)
#define RPL_OPTION_DODAG_CONFIG_PATH_CONTROL_SIZE_SHIFT     0           //!< Path control size shift (see flags field)

/**
 * @brief RPL Target Option
 * @details The RPL Target option is used to indicate a Target IPv6 address,
 * prefix, or multicast group that is reachable or queried along the
 * DODAG.  In a DAO, the RPL Target option indicates reachability.
 *
 * Option Type: 0x05
 */
struct rpl_option_rpl_target_s {
    uint8_t flags;             //!< Flags, reserved for future use
    uint8_t prefix_length;     //!< Number of leading bits in the IPv6 prefix that are valid (0 to 128)
    uint8_t prefix[];          //!< Variable length field containing an IPv6 destination address, prefix, or multicast group
} rpl_option_rpl_target_s;

//TODO: Continue here from page 54. Transit information

/**
 * @brief RPL generic option structure
 * @details All options follow this format. Except Pad1.
 */
struct rpl_option_s {
    uint8_t type;           //!< Option type (see rpl_option_type_e)
    uint8_t length;         //!< Option length
    union {
        struct rpl_option_pad1_s pad1;
        struct rpl_option_padN_s padN;
        struct rpl_option_dag_metric_s dag_metric;
        struct rpl_option_route_info_s route_info;
        struct rpl_option_rpl_target_s rpl_target;
    } data;
} rpl_option_s;

/***            RPL Security structures, flags and enumerations             ***/

/**
 * @brief RPL ICMP security section
 * @details [long description]
 *
 */
struct rpl_security_s {
    uint8_t t;                  //!< Top bit indicates whether counter field is a time stamp or incrementing counter. Further bits are initialized to 0 and ignored.
    uint8_t algorithm;          //!< Specifies the encryption, MAC, and signature scheme for the network.
    uint8_t kim_and_lvl;        //!< Key Identifier Mode (see RPL_SEC_KIM_* and rpl_security_kim_mode_e) as well as Security Level (see RPL_SEC_LVL_*).
    uint8_t flags;              //!< Unused field reserved for flags.
    uint32_t counter;           //!< Counter field, non repeating 4-octet field used to construct cryptographic mechanism for packet protection.
    union {
        struct {
            uint8_t key_index;      //!< Index used to identify different keys from the same originator
        } key_identifier_mode0;     //!< Indicates which key was used to protect the packet in Key Identifier Mode 0
        struct {
        } key_identifier_mode1;     //!< Indicates which key was used to protect the packet in Key Identifier Mode 1
        struct {
            uint8_t key_source;     //!< Indicates the logical identifier of the originator of a group key (optional field)
            uint8_t key_index;      //!< Index used to identify different keys from the same originator (optional field)
        } key_identifier_mode2;     //!< Indicates which key was used to protect the packet in Key Identifier Mode 2
        struct {
            uint8_t key_source;     //!< Indicates the logical identifier of the originator of a group key (optional field)
            uint8_t key_index;      //!< Index used to identify different keys from the same originator (optional field)
        } key_identifier_mode3;     //!< Indicates which key was used to protect the packet in Key Identifier Mode 3
    };
} rpl_security_s;


#define RPL_SECURITY_COUNTER_IS_TIME_FLAG   0x80        //!< Indicates the counter field is a time stamp

enum rpl_security_algorithm_e {
    RPL_SECURITY_ALGORITHM_CCM_AES123_RSA_SHA256 = 0    //!< CCM with AES-129 for encryption, RSA with SHA-256 for signatures
};

#define RPL_SEC_KIM_MASK                        0xE0
#define RPL_SEC_KIM_SHIFT                       5

enum rpl_security_kim_mode_e {
    RPL_SEC_KIM_MODE0 = 0x00,   //!< Group key used. Key field determined by Key Index field. Key source not present, key index present.
    RPL_SEC_KIM_MODE1 = 0x01,   //!< Per-pair key used. Key determined by source and destination. Key source not present, key index not present.
    RPL_SEC_KIM_MODE2 = 0x02,   //!< Group key used. Key determined by Key Index and Key Source Identifier. Key source is present. Key index is present.
    RPL_SEC_KIM_MODE3 = 0x03    //!< Node signature key used. If packet is encrypted, use a group key. Key Index and Key Source specify key. Key Source and Key Index may be present.
};

#define RPL_SEC_LVL_MASK                        0x07
#define RPL_SEC_LVL_SHIFT                       0

/**
 * Defines the security level for all normal key identifier modes (not mode 3)
 * This indicates the packet protection used
 */
enum rpl_security_lvl_normal_e {
    RPL_SEC_LVL_NORM_MODE0 = 0x00,  //!< MAC-32 with length 4
    RPL_SEC_LVL_NORM_MODE1 = 0x00,  //!< ENC-MAC-32 with length 4
    RPL_SEC_LVL_NORM_MODE2 = 0x00,  //!< MAC-64 with length 8
    RPL_SEC_LVL_NORM_MODE3 = 0x00,  //!< ENC-MAC-64 with length 8
};

/**
 * Defines the security level for all key identifier mode 3
 * This indicates the packet protection used
 */
enum rpl_security_lvl_mode_three_e {
    RPL_SEC_LVL_KIM3_MODE0 = 0x00,  //!< Sign-3072 with signature length 384
    RPL_SEC_LVL_KIM3_MODE1 = 0x00,  //!< ENC-Sign-3072 with signature length 384
    RPL_SEC_LVL_KIM3_MODE2 = 0x00,  //!< SIGN-2048 with signature length 256
    RPL_SEC_LVL_KIM3_MODE3 = 0x00,  //!< ENC-SIGN-2013 with signature length 2256
};

/**
 * Union combining RPL control message base structures
 * This allows simple inclusion in both standard and secure type control messages
 */
union rpl_control_message_base_u {
    struct rpl_dio_s dio;
    struct rpl_dis_s dis;
    struct rpl_dao_s dao;
    struct rpl_dao_ack_s dao_ack;
};

/**
 * @brief RPL control message structure (for ICMPv6)
 * @details [long description]
 *
 */
struct rpl_control_message_s {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    rpl_control_message_base_u base;
    void *options;
};

/**
 * @brief RPL secure control message structure (for ICMPv6)
 * @details [long description]
 *
 */
struct rpl_secure_control_message_s {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    struct rpl_security_s security;
    rpl_control_message_base_u base;
    uint8_t option_type;
    uint8_t option_length;
    uint8_t option_data[MAX_OPTION_DATA];
};



