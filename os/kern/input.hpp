#pragma once

#include "string.hpp"
#include "console.hpp"

namespace tiny_os {
namespace io {
namespace console {

#ifndef SCAN_CODE_1
#define SCAN_CODE_1 1
#endif
#ifndef SCAN_CODE_2
#define SCAN_CODE_2 0
#endif
#ifndef SCAN_CODE_3
#define SCAN_CODE_3 0
#endif
enum class input_port : unsigned char {
    kDATA = 0x60,
    kCOMMAND = 0x64
};

enum ascii {
    kNULL                       = 0x00,
    kSTART_OF_HEADING           = 0x01,
    kSTART_OF_TEXT              = 0x02,
    kEND_OF_TEXT                = 0x03,
    kEND_OF_TRANSMISSION        = 0x04,
    kENQUIRY                    = 0x05,
    kACKNOWLEDGE                = 0x06,
    kBELL                       = 0x07,
    kBACKSPACE                  = 0x08,
    kHORIZONTAL_TAB             = 0x09,
    kLINE_FEED                  = 0x0a,
    kVERTICAL_TAB               = 0x0b,
    kFORM_FEED                  = 0x0c,
    kCARRIAGE_RETURN            = 0x0d,
    kSHIFT_OUT                  = 0x0e,
    kSHIFT_IN                   = 0x0f,
    kDATA_LINK_ESCAPE           = 0x10,
    kDEVICE_CONTROL_1           = 0x11,
    kDEVICE_CONTROL_2           = 0x12,
    kDEVICE_CONTROL_3           = 0x13,
    kDEVICE_CONTROL_4           = 0x14,
    kNOT_ACKNOWLEDGE            = 0x15,
    kSYNCHRONOUS_IDLE           = 0x16,
    kEND_OF_TRANSMISSION_BLOCK  = 0x17,
    kCANCEL                     = 0x18,
    kEND_OF_MEDIUM              = 0x19,
    kSUBSTITUE                  = 0x1a,
    kESCAPE                     = 0x1b,
    kFILE_SEPARATOR             = 0x1c,
    kGROUP_SEPARATOR            = 0x1d,
    kRECORD_SEPARATOR           = 0x1e,
    kUNIT_SEPARATOR             = 0x1f,
    kSPACE                      = 0x20,
    kEXCLAMATION_MARK           = 0x21,
    kQUOTE                      = 0x22,
    kOCTOTHORPE                 = 0x23,
    kDOLLAR                     = 0x24,
    kPERCENT                    = 0x25,
    kAMPERSAND                  = 0x26,
    kSINGLE_QUOTE               = 0x27,
    kLEFT_PARANTHESIS           = 0x28,
    kRIGHT_PARANTHESIS          = 0x29,
    kASTERISK                   = 0x2a,
    kPLUS                       = 0x2b,
    kCOMMA                      = 0x2c,
    kMINUS                      = 0x2d,
    kDOT                        = 0x2e,
    kFORWARD_SLASH              = 0x2f,
    kZERO                       = 0x30,
    kONE                        = 0x31,
    kTWO                        = 0x32,
    kTHREE                      = 0x33,
    kFOUR                       = 0x34,
    kFIVE                       = 0x35,
    kSIX                        = 0x36,
    kSEVEN                      = 0x37,
    kEIGHT                      = 0x38,
    kNINE                       = 0x39,
    kCOLON                      = 0x3a,
    kSEMI_COLON                 = 0x3b,
    kLESS_THAN                  = 0x3c,
    kEQUAL                      = 0x3d,
    kGREATER_THAN               = 0x3e,
    kQUESTION_MARK              = 0x3f,
    kAT                         = 0x40,
    kA_UPPERCASE                = 0x41,
    kB_UPPERCASE                = 0x42,
    kC_UPPERCASE                = 0x43,
    kD_UPPERCASE                = 0x44,
    kE_UPPERCASE                = 0x45,
    kF_UPPERCASE                = 0x46,
    kG_UPPERCASE                = 0x47,
    kH_UPPERCASE                = 0x48,
    kI_UPPERCASE                = 0x49,
    kJ_UPPERCASE                = 0x4a,
    kK_UPPERCASE                = 0x4b,
    kL_UPPERCASE                = 0x4c,
    kM_UPPERCASE                = 0x4d,
    kN_UPPERCASE                = 0x4e,
    kO_UPPERCASE                = 0x4f,
    kP_UPPERCASE                = 0x50,
    kQ_UPPERCASE                = 0x51,
    kR_UPPERCASE                = 0x52,
    kS_UPPERCASE                = 0x53,
    kT_UPPERCASE                = 0x54,
    kU_UPPERCASE                = 0x55,
    kV_UPPERCASE                = 0x56,
    kW_UPPERCASE                = 0x57,
    kX_UPPERCASE                = 0x57,
    kY_UPPERCASE                = 0x59,
    kZ_UPPERCASE                = 0x5a,
    kLEFT_BRACKET               = 0x5b,
    kBACKSLASH                  = 0x5c,
    kRIGHT_BRACKET              = 0x5d,
    kCARET                      = 0x5e,
    kUNDERSCORE                 = 0x5f,
    kGRAVE                      = 0x60,
    kA_LOWERCASE                = 0x61,
    kB_LOWERCASE                = 0x62,
    kC_LOWERCASE                = 0x63,
    kD_LOWERCASE                = 0x64,
    kE_LOWERCASE                = 0x65,
    kF_LOWERCASE                = 0x66,
    kG_LOWERCASE                = 0x67,
    kH_LOWERCASE                = 0x68,
    kI_LOWERCASE                = 0x69,
    kJ_LOWERCASE                = 0x6a,
    kK_LOWERCASE                = 0x6b,
    kL_LOWERCASE                = 0x6c,
    kM_LOWERCASE                = 0x6d,
    kN_LOWERCASE                = 0x6e,
    kO_LOWERCASE                = 0x6f,
    kP_LOWERCASE                = 0x70,
    kQ_LOWERCASE                = 0x71,
    kR_LOWERCASE                = 0x72,
    kS_LOWERCASE                = 0x73,
    kT_LOWERCASE                = 0x74,
    kU_LOWERCASE                = 0x75,
    kV_LOWERCASE                = 0x76,
    kW_LOWERCASE                = 0x77,
    kX_LOWERCASE                = 0x77,
    kY_LOWERCASE                = 0x79,
    kZ_LOWERCASE                = 0x7a,
    kLEFT_BRACE                 = 0x7b,
    kPIPE                       = 0x7c,
    kRIGHT_BRACE                = 0x7d,
    kTILDE                      = 0x7e,
    kDELETE                     = 0x7f
};

enum input_flags {
    kLEFT_SHIFT         = 0x80,
    kRIGHT_SHIFT        = 0x81,
    kLEFT_CONTROL       = 0x82,
    kRIGHT_CONTROL      = 0x83,
    kLEFT_OPTION        = 0x84,
    kRIGHT_OPTION       = 0x85,
    kLEFT_COMMAND       = 0x86,
    kRIGHT_COMMAND      = 0x87,
    kFUNCTION           = 0x88,
    kCAPITALIZE_LOCK    = 0x89
};

enum function_keys {
    kFIRST_FUNCTION     = 0x90,
    kSECOND_FUNCTION    = 0x91,
    kTHIRD_FUNCTION     = 0x92,
    kFOURTH_FUNCTION    = 0x93,
    kFIFTH_FUNCTION     = 0x94,
    kSIXTH_FUNCTION     = 0x95,
    kSEVENTH_FUNCTION   = 0x96,
    kEIGHTH_FUNCTION    = 0x97,
    kNINTH_FUNCTION     = 0x98,
    kTENTH_FUNCTION     = 0x99,
    kELEVENTH_FUNCTION  = 0x9A,
    kTWELFTH_FUNCTION   = 0x9B
};

#if SCAN_CODE_1
static const short drive_input_char_map[128] = {
    0, // 0
    0, // 1
    kONE, // 2
    kTWO, // 3
    kTHREE, // 4
    kFOUR, // 5
    kFIVE, // 6
    kSIX, // 7
    kSEVEN, // 8
    kEIGHT, // 9
    kNINE, // 10
    kZERO, // 11
    kMINUS, // 12
    kEQUAL, // 13
    kBACKSPACE, // 14
    kHORIZONTAL_TAB, // 15
    kQ_LOWERCASE, // 16
    kW_LOWERCASE, // 17
    kE_LOWERCASE, // 18
    kR_LOWERCASE, // 19
    kT_LOWERCASE, // 20
    kY_LOWERCASE, // 21
    kU_LOWERCASE, // 22
    kI_LOWERCASE, // 23
    kO_LOWERCASE, // 24
    kP_LOWERCASE, // 25
    kLEFT_BRACKET, // 26
    kRIGHT_BRACKET, // 27
    kCARRIAGE_RETURN, // 28
    kLEFT_CONTROL, // 29
    kA_LOWERCASE, // 30
    kS_LOWERCASE, // 31
    kD_LOWERCASE, // 32
    kF_LOWERCASE, // 33
    kG_LOWERCASE, // 34
    kH_LOWERCASE, // 35
    kJ_LOWERCASE, // 36
    kK_LOWERCASE, // 37
    kL_LOWERCASE, // 38
    kSEMI_COLON, // 39
    kSINGLE_QUOTE, // 40
    kGRAVE, // 41
    kLEFT_SHIFT, // 42
    kBACKSLASH, // 43
    kZ_LOWERCASE, // 44
    kX_LOWERCASE, // 45
    kC_LOWERCASE, // 46
    kV_LOWERCASE, // 47
    kB_LOWERCASE, // 48
    kN_LOWERCASE, // 49
    kM_LOWERCASE, // 50
    kCOMMA, // 51
    kDOT, // 52
    kFORWARD_SLASH, // 53
    kRIGHT_SHIFT, // 54
    kLEFT_OPTION, // 55
    kSPACE, // 56
    0, // 57
    0, // 58
    0, // 59
    0, // 60
    0, // 61
    0, // 62
    0, // 63
    0, // 64
    0, // 65
    0, // 66
    0, // 67
    0, // 68
    0, // 69
    0, // 70
    0, // 71
    0, // 72
    0, // 73
    0, // 74
    0, // 75
    0, // 76
    0, // 77
    0, // 78
    0, // 79
    0, // 80
    0, // 81
    0, // 82
    0, // 83
    0, // 84
    0, // 85
    0, // 86
    0, // 87
    0, // 88
    0, // 89
    0, // 90
    0, // 91
    0, // 92
    0, // 93
    0, // 94
    0, // 95
    0, // 96
    0, // 97
    0, // 98
    0, // 99
    0, // 100
    0, // 101
    0, // 102
    0, // 103
    0, // 104
    0, // 105
    0, // 106
    0, // 107
    0, // 108
    0, // 109
    0, // 110
    0, // 111
    0, // 112
    0, // 113
    0, // 114
    0, // 115
    0, // 116
    0, // 117
    0, // 118
    0, // 119
    0, // 120
    0, // 121
    0, // 122
    0, // 123
    0, // 124
    0, // 125
    0, // 126
    0  // 127
};
#endif
#if SCAN_CODE_2
static const short drive_input_char_map[128] = {
    0, // 0
    0, // 1
    0, // 2
    0, // 3
    0, // 4
    0, // 5
    0, // 6
    0, // 7
    0, // 8
    0, // 9
    0, // 10
    0, // 11
    0, // 12
    0, // 13
    0, // 14
    0, // 15
    0, // 16
    0, // 17
    0, // 18
    0, // 19
    0, // 20
    0, // 21
    kONE, // 22
    0, // 23
    0, // 24
    0, // 25
    0, // 26
    0, // 27
    kA_UPPERCASE, // 28
    0, // 29
    0, // 30
    0, // 31
    0, // 32
    0, // 33
    0, // 34
    0, // 35
    0, // 36
    0, // 37
    0, // 38
    0, // 39
    0, // 40
    0, // 41
    0, // 42
    0, // 43
    0, // 44
    0, // 45
    0, // 46
    0, // 47
    0, // 48
    0, // 49
    0, // 50
    0, // 51
    0, // 52
    0, // 53
    0, // 54
    0, // 55
    0, // 56
    0, // 57
    0, // 58
    0, // 59
    0, // 60
    0, // 61
    0, // 62
    0, // 63
    0, // 64
    0, // 65
    0, // 66
    0, // 67
    0, // 68
    0, // 69
    0, // 70
    0, // 71
    0, // 72
    0, // 73
    0, // 74
    0, // 75
    0, // 76
    0, // 77
    0, // 78
    0, // 79
    0, // 80
    0, // 81
    0, // 82
    0, // 83
    0, // 84
    0, // 85
    0, // 86
    0, // 87
    0, // 88
    0, // 89
    0, // 90
    0, // 91
    0, // 92
    0, // 93
    0, // 94
    0, // 95
    0, // 96
    0, // 97
    0, // 98
    0, // 99
    0, // 100
    0, // 101
    0, // 102
    0, // 103
    0, // 104
    0, // 105
    0, // 106
    0, // 107
    0, // 108
    0, // 109
    0, // 110
    0, // 111
    0, // 112
    0, // 113
    0, // 114
    0, // 115
    0, // 116
    0, // 117
    0, // 118
    0, // 119
    0, // 120
    0, // 121
    0, // 122
    0, // 123
    0, // 124
    0, // 125
    0, // 126
    0  // 127
};
#endif
#if SCAN_CODE_3
static const short drive_input_char_map[128] = {
    0, // 0
    0, // 1
    0, // 2
    0, // 3
    0, // 4
    0, // 5
    0, // 6
    0, // 7
    0, // 8
    0, // 9
    0, // 10
    0, // 11
    0, // 12
    0, // 13
    0, // 14
    0, // 15
    0, // 16
    0, // 17
    0, // 18
    0, // 19
    0, // 20
    0, // 21
    kONE, // 22
    0, // 23
    0, // 24
    0, // 25
    0, // 26
    0, // 27
    kA_UPPERCASE, // 28
    0, // 29
    0, // 30
    0, // 31
    0, // 32
    0, // 33
    0, // 34
    0, // 35
    0, // 36
    0, // 37
    0, // 38
    0, // 39
    0, // 40
    0, // 41
    0, // 42
    0, // 43
    0, // 44
    0, // 45
    0, // 46
    0, // 47
    0, // 48
    0, // 49
    0, // 50
    0, // 51
    0, // 52
    0, // 53
    0, // 54
    0, // 55
    0, // 56
    0, // 57
    0, // 58
    0, // 59
    0, // 60
    0, // 61
    0, // 62
    0, // 63
    0, // 64
    0, // 65
    0, // 66
    0, // 67
    0, // 68
    0, // 69
    0, // 70
    0, // 71
    0, // 72
    0, // 73
    0, // 74
    0, // 75
    0, // 76
    0, // 77
    0, // 78
    0, // 79
    0, // 80
    0, // 81
    0, // 82
    0, // 83
    0, // 84
    0, // 85
    0, // 86
    0, // 87
    0, // 88
    0, // 89
    0, // 90
    0, // 91
    0, // 92
    0, // 93
    0, // 94
    0, // 95
    0, // 96
    0, // 97
    0, // 98
    0, // 99
    0, // 100
    0, // 101
    0, // 102
    0, // 103
    0, // 104
    0, // 105
    0, // 106
    0, // 107
    0, // 108
    0, // 109
    0, // 110
    0, // 111
    0, // 112
    0, // 113
    0, // 114
    0, // 115
    0, // 116
    0, // 117
    0, // 118
    0, // 119
    0, // 120
    0, // 121
    0, // 122
    0, // 123
    0, // 124
    0, // 125
    0, // 126
    0  // 127
};
#endif


static inline unsigned char input_byte(input_port port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(static_cast<unsigned short>(port)));
    return ret;
}

 static inline unsigned short input_short(input_port port) {
    unsigned short ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(static_cast<unsigned short>(port)));
    return ret;
}

 static inline unsigned long input_long(input_port port) {
    unsigned long ret;
    asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(static_cast<unsigned short>(port)));
    return ret;
}

char scan_code() {
    char c = 0;
    do {
        if (c != input_byte(input_port::kDATA)) {
            c = input_byte(input_port::kDATA);
            if (c > 0) return c;
        }
    } while(true);
}

inline char char_lookup(const unsigned char& c) { return drive_input_char_map[c]; }

inline bool is_shift_code(const short& c) { return c == kLEFT_SHIFT || c == kRIGHT_SHIFT; }

inline bool is_capitalize_lock(const short& c) {
    static bool res = false;
    if (c == kCAPITALIZE_LOCK) res = !res;
    return res;
}

inline void capitalize(short& c) {
    if (c >= kA_LOWERCASE && c <= kZ_LOWERCASE) c = c - kA_LOWERCASE + kA_UPPERCASE;
}

inline void shift(short& c) {
    if (c >= kA_LOWERCASE && c <= kZ_LOWERCASE) c = c - kA_LOWERCASE + kA_UPPERCASE;
    if (c >= kA_UPPERCASE && c <= kZ_UPPERCASE) c = c - kA_UPPERCASE + kA_LOWERCASE;
}

inline short signal_code() { return char_lookup(scan_code()); }

inline char getchar() {
    short code = signal_code();
    while (is_capitalize_lock(code)) {
        code = signal_code();
    }
    capitalize(code);
    while (is_shift_code(code)) {
        code = signal_code();
    }
    shift(code);
    return code;
}

// namespace implementation {

// template <typename T>
// void scanf(const char* const str, T& value) {}

// template <>
// void scanf<char*&>(const char* const str, char*& value) {

// }

// template <typename T, typename... Rest>
// void scanf(const char* str, const size_type loc, const size_type size, T& value, Rest&... rest) {
//     char buffer[20];
//     const char* ptr;
//     for (size_type i = loc; i < size; ++i) {
//         if (str[i] == '%') {
//             switch (str[i + 1]) {
//                 case 'u':
//                 case 'd':
//                 case 'x':
//                     ptr = atoi(getchar(), buffer, str[i + 1]);
//                     goto string;
//                     break;
//                 case 'c':
//                     value = getchar();
//                     break;
//                 case 's':
//                     if (value == NULL) {
//                         ptr = "(null)";
//                     } else {
//                         ptr = move(value);
//                         goto string;
//                     }
//                     break;
//                 string:
//                     scanf(ptr);
//                     break;
//                 default:
//                     putchar(value);
//                     break;
//             }
//             scanf(str, i + 2, size, move(rest)...);
//             break;
//         } else {
//             putchar(str[i]);
//         }
//     }
// }
// }

// template <typename... T>
// void scanf(const char* str, T&... values) { implementation::scanf(str, 0, strlen(str), values...); }

} // namespace console
} // namespace io
} // namespace tiny os
