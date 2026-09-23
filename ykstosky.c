#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Этот сваппер был написан на основе анализа Skyrim.esm . А он содержит текстовые ресурсы переводов на языки в файлах .STRINGS , .DLSTRINGS , ILSTRINGS , из-за чего некоторые переменные представляют из себя ссылки на них. Т.е., по кр. мере для сваппинга плагинов потребуется подвести их к идентичной структуре с STRINGS, что для чего может потребоваться прогнать их через Skyrim String Localizer.

// Эта сигнатура видимо специфично переносится под PS3. Сравнивать с идентичными, и точечно править байты.
// CTDA - отличается в первой четвёрке байт и в половине в третьей четвёрке байт.

static uint32_t processed_glogal_signature = 0;

const uint32_t global_signature_types[] = {
    0x41414354, // AACT
    0x41434852, // ACHR
    0x41435449, // ACTI
    0x4144444E, // ADDN
    0x414C4348, // ALCH
    0x414D4D4F, // AMMO
    0x414E494F, // ANIO
    0x41505041, // APPA
    0x41524D41, // ARMA
    0x41524D4F, // ARMO
    0x4152544F, // ARTO
    0x41535043, // ASPC
    0x41535450, // ASTP
    0x41564946, // AVIF
    0x424F4F4B, // BOOK
    0x42505444, // BPTD
    0x43414D53, // CAMS
    0x43454C4C, // CELL
    0x434C4153, // CLAS
	0x434C4443, // CLDC
    0x434C464D, // CLFM
    0x434C4D54, // CLMT
    0x434F424A, // COBJ
    0x434F4C4C, // COLL
    0x434F4E54, // CONT
    0x43505448, // CPTH
    0x43535459, // CSTY
	0x44415441, // DATA
    0x44454252, // DEBR
    0x4449414C, // DIAL
    0x444C4252, // DLBR
    0x444C5657, // DLVW
    0x444F424A, // DOBJ
    0x444F4F52, // DOOR
    0x4455414C, // DUAL
    0x45435A4E, // ECZN
    0x45465348, // EFSH
    0x454E4348, // ENCH
    0x45515550, // EQUP
    0x4558504C, // EXPL
    0x45594553, // EYES
    0x46414354, // FACT
    0x464C4F52, // FLOR
    0x464C5354, // FLST
    0x46535450, // FSTP
    0x46535453, // FSTS
    0x4655524E, // FURN
    0x474C4F42, // GLOB
    0x474D5354, // GMST
    0x47524153, // GRAS
	0x48414952, // HAIR
//    0x47525550, // GRUP excluded cause it is for it
    0x48415A44, // HAZD
    0x48445054, // HDPT
	0x48454452, // HEDR
    0x49444C45, // IDLE
    0x49444C4D, // IDLM
    0x494D4144, // IMAD
    0x494D4753, // IMGS
    0x494E464F, // INFO
    0x494E4752, // INGR
    0x49504354, // IPCT
    0x49504453, // IPDS
    0x4B45594D, // KEYM
    0x4B595744, // KYWD
    0x4C414E44, // LAND
    0x4C435254, // LCRT
    0x4C43544E, // LCTN
    0x4C47544D, // LGTM
    0x4C494748, // LIGH
    0x4C534352, // LSCR
    0x4C544558, // LTEX
    0x4C564C49, // LVLI
    0x4C564C4E, // LVLN
    0x4C565350, // LVSP
    0x4D41544F, // MATO
    0x4D415454, // MATT
    0x4D455347, // MESG
    0x4D474546, // MGEF
    0x4D495343, // MISC
    0x4D4F5654, // MOVT
    0x4D535454, // MSTT
    0x4D555343, // MUSC
    0x4D555354, // MUST
    0x4E415649, // NAVI
    0x4E41564D, // NAVM
    0x4E50435F, // NPC_
    0x4F544654, // OTFT
    0x5041434B, // PACK
    0x5045524B, // PERK
    0x50475245, // PGRE
    0x50485A44, // PHZD
    0x50524F4A, // PROJ
	0x50574154, // PWAT
    0x51555354, // QUST
    0x52414345, // RACE
    0x52454652, // REFR
    0x5245474E, // REGN
    0x52454C41, // RELA
    0x52455642, // REVB
    0x54434652, // RFCT
    0x5343454E, // SCEN
	0x53434F4C, // SCOL
	0x53435054, // SCPT
    0x5343524C, // SCRL
    0x53484F55, // SHOU
    0x534C474D, // SLGM
    0x534D424E, // SMBN
    0x534D454E, // SMEN
    0x534D514E, // SMQN
    0x534E4354, // SNCT
    0x534E4452, // SNDR
    0x534F504D, // SOPM
    0x534F554E, // SOUN
    0x5350454C, // SPEL
    0x53504744, // SPGD
    0x53544154, // STAT
    0x54414354, // TACT
//    0x54455334, // TES4 excluded
    0x54524545, // TREE
    0x54585354, // TXST
    0x56545950, // VTYP
    0x57415452, // WATR
    0x57454150, // WEAP
    0x574F4F50, // WOOP
    0x57524C44, // WRLD
    0x57544852 // WTHR
};

const uint32_t signature_types[] = {
    0x41414354, // AACT ^
    0x41434852, // ACHR ^  <-- not first_next
    0x41435449, // ACTI ^
    0x4144444E, // ADDN ^
    0x414C4348, // ALCH ^
    0x414D4D4F, // AMMO ^
    0x414E494F, // ANIO ^
    0x41505041, // APPA ^
    0x41524D41, // ARMA ^
    0x41524D4F, // ARMO ^
    0x4152544F, // ARTO ^
    0x41535043, // ASPC ^
    0x41535450, // ASTP ^
    0x41564946, // AVIF ^
    0x424F4F4B, // BOOK ^
    0x42505444, // BPTD ^
    0x43414D53, // CAMS ^
    0x43454C4C, // CELL ^  <-- not first_next
    0x434C4153, // CLAS ^ ?? next one is CLDL but it is absent in Skyrim.esm
    0x434C464D, // CLFM ^
    0x434C4D54, // CLMT ^
    0x434F424A, // COBJ ^
    0x434F4C4C, // COLL ^
    0x434F4E54, // CONT ^
    0x43505448, // CPTH ^
    0x43535459, // CSTY ^
	0x44415441, // DATA ^ <-- not first_next
    0x44454252, // DEBR ^
    0x4449414C, // DIAL ^
    0x444C4252, // DLBR ^
    0x444C5657, // DLVW ^
    0x444F424A, // DOBJ ^
    0x444F4F52, // DOOR ^
    0x4455414C, // DUAL ^
    0x45435A4E, // ECZN ^
    0x45465348, // EFSH ^
    0x454E4348, // ENCH ^
    0x45515550, // EQUP ^
    0x4558504C, // EXPL ^
    0x45594553, // EYES ^
    0x46414354, // FACT ^
    0x464C4F52, // FLOR ^
    0x464C5354, // FLST ^
    0x46535450, // FSTP ^
    0x46535453, // FSTS ^
    0x4655524E, // FURN ^
    0x474C4F42, // GLOB ^
    0x474D5354, // GMST ^
    0x47524153, // GRAS ^
	0x48414952, // HAIR ^ <-- not first_next
    0x47525550, // GRUP ^ <-- not first_next
    0x48415A44, // HAZD ^
    0x48445054, // HDPT ^
	0x48454452, // HEDR ^ <-- not first_next
    0x49444C45, // IDLE ^
    0x49444C4D, // IDLM ^
    0x494D4144, // IMAD ^
    0x494D4753, // IMGS ^
    0x494E464F, // INFO ^ <-- not first_next
    0x494E4752, // INGR ^
    0x49504354, // IPCT ^
    0x49504453, // IPDS ^
    0x4B45594D, // KEYM ^
    0x4B595744, // KYWD ^
    0x4C414E44, // LAND ^ <-- not first_next
    0x4C435254, // LCRT ^
    0x4C43544E, // LCTN ^
    0x4C47544D, // LGTM ^
    0x4C494748, // LIGH ^
    0x4C534352, // LSCR ^
    0x4C544558, // LTEX ^
    0x4C564C49, // LVLI ^
    0x4C564C4E, // LVLN ^
    0x4C565350, // LVSP ^
    0x4D41544F, // MATO ^
    0x4D415454, // MATT ^
    0x4D455347, // MESG ^
    0x4D474546, // MGEF ^
    0x4D495343, // MISC ^
    0x4D4F5654, // MOVT ^
    0x4D535454, // MSTT ^
    0x4D555343, // MUSC ^
    0x4D555354, // MUST ^
    0x4E415649, // NAVI ^
    0x4E41564D, // NAVM ^ <-- not first_next
    0x4E50435F, // NPC_ ^ <-- not first_next
    0x4F544654, // OTFT ^
    0x5041434B, // PACK ^
    0x5045524B, // PERK ^
    0x50475245, // PGRE ^  <-- not first_next
    0x50485A44, // PHZD ^  <-- not first_next
    0x50524F4A, // PROJ ^ ?? После PROJ идёт PWAT
    0x51555354, // QUST ^
    0x52414345, // RACE ^
    0x52454652, // REFR ^  <-- not first_next
    0x5245474E, // REGN ^
    0x52454C41, // RELA ^
    0x52455642, // REVB ^
    0x54434652, // RFCT ^
    0x5343454E, // SCEN ^  <-- not first_next
    0x5343524C, // SCRL ^
    0x53484F55, // SHOU ^
    0x534C474D, // SLGM ^
    0x534D424E, // SMBN ^
    0x534D454E, // SMEN ^
    0x534D514E, // SMQN ^
    0x534E4354, // SNCT ^
    0x534E4452, // SNDR ^
    0x534F504D, // SOPM ^
    0x534F554E, // SOUN ^
    0x5350454C, // SPEL ^
    0x53504744, // SPGD ^
    0x53544154, // STAT ^
    0x54414354, // TACT ^
    0x54455334, // TES4 ^ <-- not first_next
    0x54524545, // TREE ^
    0x54585354, // TXST ^
    0x56545950, // VTYP ^
    0x57415452, // WATR ^
    0x57454150, // WEAP ^
    0x574F4F50, // WOOP ^
    0x57524C44, // WRLD ^
    0x57544852, // WTHR ^
// Плохо замечаемые с мододельством
	0x4E414D45, // NAME,
	0x4E414D30, // NAM0,
	0x4E414D31, // NAM1,
	0x4E414D32, // NAM2,
	0x4E414D33, // NAM3,
	0x4E414D34, // NAM4,
	0x4E414D35, // NAM5,
	0x4E414D36, // NAM6,
	0x4E414D37, // NAM7,
	0x4E414D38, // NAM8,
	0x4E414D39, // NAM9,
	0x414E414D, // ANAM,
	0x424E414D, // BNAM,
	0x434E414D, // CNAM,
	0x444E414D, // DNAM,
	0x454E414D, // ENAM,
	0x464E414D, // FNAM,
	0x474E414D, // GNAM,
	0x484E414D, // HNAM,
	0x494E414D, // INAM,
	0x4A4E414D, // JNAM,
	0x4B4E414D, // KNAM,
	0x4C4E414D, // LNAM,
	0x4D4E414D, // MNAM,
	0x4E4E414D, // NNAM,
	0x4F4E414D, // ONAM,
	0x504E414D, // PNAM,
	0x514E414D, // QNAM,
	0x524E414D, // RNAM,
	0x534E414D, // SNAM,
	0x544E414D, // TNAM,
	0x554E414D, // UNAM,
	0x564E414D, // VNAM,
	0x574E414D, // WNAM,
	0x584E414D, // XNAM,
	0x594E414D, // YNAM,
	0x5A4E414D, // ZNAM,
	0x46554C4C, // FULL
	0x4D4F444C, // MODL
	0x464C5456, // FLTV
	0x44455343, // DESC
	0x52444154, // RDAT
	0x4F424E44, // OBND
	0x54494643, // TIFC
	0x54524454, // TRDT
	0x43544441, // CTDA
	0x564D4144, // VMAD
	0x544F4654, // TOFT
	0x554E4553, // UNES
	0x4B53495A, // KSIZ
	0x4D544E4D, // MTNM
	0x5654434B, // VTCK
	0x48434C46, // HCLF
	0x54494E4C, // TINL
	0x41544B44, // ATKD
	0x41544B45, // ATKE
	0x494E4458, // INDX
	0x50485754, // PHWT
	0x4D504149, // MPAI
	0x4D504156, // MPAV
	0x4148434D, // AHCM
	0x54494E49, // TINI
	0x54494E43, // TINC
	0x54494E54, // TINT
	0x54494E50, // TINP
	0x54494E56, // TINV
	0x54495253, // TIRS
	0x41484346, // AHCF
	0x54494E44, // TIND
	0x53504C4F, // SPLO
	0x424F4454, // BODT
	0x43525641, // CRVA
	0x444D494E, // DMIN
	0x444D4158, // DMAX
	0x48544944, // HTID
	0x44455641, // DEVA
	0x414C4944, // ALID
	0x50524B45, // PRKE
	0x504B4454, // PKDT
	0x504B4355, // PKCU
	0x504C4454, // PLDT
	0x5044544F, // PDTO
	0x504F4341, // POCA
	0x504F4541, // POEA
	0x504F4241, // POBA
	0x50534454, // PSDT
	0x43495332, // CIS2
	0x4549544D, // EITM
	0x434E544F, // CNTO
	0x434F4354, // COCT
	0x56454E56, // VENV
	0x494E414D, // INAM
	0x58505041, // XPPA
	0x5850524D, // XPRM
	0x584D424F, // XMBO
	0x58505244, // XPRD
	0x584C524D, // XLRM
	0x584E4450, // XNDP
	0x584C434E, // XLCN
	0x5854454C, // XTEL
	0x5853434C, // XSCL
	0x4E455854, // NEXT
	0x44454D4F, // DEMO
	0x58524744, // XRGD
	0x464C5452, // FLTR
	0x444F4454, // DODT
	0x56454E44, // VEND
	0x56454E43, // VENC
	0x504C5644, // PLVD
	0x57414954, // WAIT
	0x53544F4C, // STOL
	0x504C434E, // PLCN
	0x4A4F5554, // JOUT
	0x43524752, // CRGR
	0x58524453, // XRDS
	0x4D4F4454, // MODT
	0x534E4444, // SNDD
	0x53445343, // SDSC
	0x4D444F42, // MDOB
	0x4B574441, // KWDA
	0x4A41494C, // JAIL
	0x43495443, // CITC
	0x49434F4E, // ICON
	0x48454144, // HEAD
	0x5250524D, // RPRM
	0x52505246, // RPRF
	0x574B4D56, // WKMV
	0x514F424A, // QOBJ
	0x51535441, // QSTA
	0x414C5354, // ALST
	0x414C5043, // ALPC
	0x58415052, // XAPR
	0x58415044, // XAPD
	0x58455350, // XESP
	0x584F574E, // XOWN
	0x584C4947, // XLIG
	0x58454D49, // XEMI
	0x584C4B52, // XLKR
	0x584C5254, // XLRT
	0x49434F32, // ICO2
	0x53504354, // SPCT
	0x46545346, // FTSF
	0x5048544E, // PHTN
	0x524E4D56, // RNMV
	0x4D545950, // MTYP
	0x4654534D, // FTSM
	0x45545950, // ETYP
	0x45464954, // EFIT
	0x45464944, // EFID
	0x44535446, // DSTF
	0x42414D54, // BAMT
	0x52444F54, // RDOT
	0x52504C49, // RPLI
	0x52434C52, // RCLR
	0x414C4544, // ALED
	0x414C434C, // ALCL
	0x414C4E41, // ALNA
	0x414C4645, // ALFE
	0x414C434F, // ALCO
	0x414C4643, // ALFC
	0x414C5541, // ALUA
	0x58504F44, // XPOD
	0x584C4942, // XLIB
	0x584C434D, // XLCM
	0x58505752, // XPWR
	0x58434C57, // XCLW
	0x58434C43, // XCLC
	0x4C544D50, // LTMP
	0x44465446, // DFTF
	0x53574D56, // SWMV
	0x53504544, // SPED
	0x4446544D, // DFTM
	0x444D4454, // DMDT
	0x4D4F4453, // MODS
	0x444D444C, // DMDL
	0x44535444, // DSTD
	0x45414D54, // EAMT
	0x42494453, // BIDS
	0x43524454, // CRDT
	0x4E564D49, // NVMI
	0x58524D52, // XRMR
	0x584C4F43, // XLOC
	0x5857434E, // XWCN
	0x58484F52, // XHOR
	0x584D524B, // XMRK
	0x54434C54, // TCLT
	0x54574154, // TWAT
	0x414C4C53, // ALLS
	0x414C4E54, // ALNT
	0x414C4644, // ALFD
	0x414C4652, // ALFR
	0x49444C41, // IDLA
	0x50544441, // PTDA
	0x50524342, // PRCB
	0x43534352, // CSCR
	0x4353464C, // CSFL
	0x43534744, // CSGD
	0x4C564C4F, // LVLO
	0x50524B43, // PRKC
	0x58414C50, // XALP
	0x58434E54, // XCNT
	0x58545249, // XTRI
	0x58434C52, // XCLR
	0x49444C46, // IDLF
	0x49444C43, // IDLC
	0x49444C54, // IDLT
	0x49444C4D, // IDLM
	0x464C4D56, // FLMV
	0x534E4D56, // SNMV
	0x454E4954, // ENIT
	0x53504954, // SPIT
	0x44455354, // DEST
	0x444D4453, // DMDS
	0x5155414C, // QUAL
	0x50465043, // PFPC
	0x50464947, // PFIG
	0x464E5052, // FNPR
	0x57424454, // WBDT
	0x4C564C46, // LVLF
	0x4C564C44, // LVLD
	0x414C4341, // ALCA
	0x51534454, // QSDT
	0x414C464C, // ALFL
	0x414C4641, // ALFA
	0x414C444E, // ALDN
	0x5843494D, // XCIM
	0x584D4252, // XMBR
	0x58455A4E, // XEZN
	0x58435A43, // XCZC
	0x4D4F4432, // MOD2
	0x4D4F4433, // MOD3
	0x4D4F4434, // MOD4
	0x4D4F4435, // MOD5
	0x4D4F3254, // MO2T
	0x4D4F3354, // MO3T
	0x4D4F3454, // MO4T
	0x4D4F3554, // MO5T
	0x434F4544, // COED
	0x464E4D4B, // FNMK
	0x4C564C47, // LVLG
	0x4C4C4354, // LLCT
	0x494D5350, // IMSP
	0x44414C43, // DALC
	0x52504C44, // RPLD
	0x52445754, // RDWT
	0x52445341, // RDSA
	0x52444D50, // RDMP
	0x52444D4F, // RDMO
	0x58574355, // XWCU
	0x4E414D41, // NAMA
	0x58544E4D, // XTNM
	0x45434F52, // ECOR
	0x414C5254, // ALRT
	0x414C4551, // ALEQ
	0x5154474C, // QTGL
	0x414C4649, // ALFI
	0x43534D45, // CSME
	0x43534C52, // CSLR
	0x45504654, // EPFT
	0x50524B46, // PRKF
	0x42504E49, // BPNI
	0x4250544E, // BPTN
	0x42504E4E, // BPNN
	0x42504E54, // BPNT
	0x4C534352, // LSCR
	0x4C434543, // LCEC
	0x4C434944, // LCID
	0x4C434550, // LCEP
	0x42504E44, // BPND
	0x534F554C, // SOUL
	0x574C5354, // WLST
	0x4E564552, // NVER
	0x58414354, // XACT
	0x58465643, // XFVC
	0x414C4541, // ALEA
	0x414C5350, // ALSP
	0x53504F52, // SPOR
	0x45504644, // EPFD
	0x4156534B, // AVSK
	0x4C435052, // LCPR
	0x49545854, // ITXT
	0x4D505254, // MPRT
	0x46505254, // FPRT
	0x46434854, // FCHT
	0x4D434854, // MCHT
	0x58524E4B, // XRNK
	0x534C4350, // SLCP
	0x57435452, // WCTR
	0x4F465354, // OFST
	0x4C43554E, // LCUN
	0x58434153, // XCAS
	0x58434D4F, // XCMO
	0x4D4F3253, // MO2S
	0x4D4F3353, // MO3S
	0x4D4F3453, // MO4S
	0x58434C4C, // XCLL
	0x54564454, // TVDT
	0x4D484454, // MHDT
	0x58524742, // XRGB
	0x504B4332, // PKC2
	0x5843564C, // XCVL
	0x58435A41, // XCZA
	0x58535043, // XSPC
	0x58495332, // XIS2
	0x58485457, // XHTW
	0x43495331, // CIS1
	0x4C435352, // LCSR
	0x45504632, // EPF2
	0x45504633, // EPF3
	0x54583030, // TX00
	0x54583031, // TX01
	0x54583032, // TX02
	0x54583033, // TX03
	0x54583034, // TX04
	0x54583035, // TX05
	0x54583036, // TX06
	0x54583037, // TX07
	0x30305458, // 00TX
	0x31305458, // 10TX
	0x32305458, // 20TX
	0x33305458, // 30TX
	0x34305458, // 40TX
	0x35305458, // 50TX
	0x36305458, // 60TX
	0x37305458, // 70TX
	0x38305458, // 80TX
	0x39305458, // 90TX
	0x3A305458, // :0TX
	0x3B305458, // ;0TX
	0x3C305458, // <0TX
	0x3D305458, // =0TX
	0x3E305458, // >0TX
	0x3F305458, // ?0TX
	0x40305458, // @0TX
	0x41305458, // A0TX
	0x42305458, // B0TX
	0x43305458, // C0TX
	0x44305458, // D0TX
	0x45305458, // E0TX
	0x46305458, // F0TX
	0x47305458, // G0TX
	0x48305458, // H0TX
	0x49305458, // I0TX
	0x4A305458, // J0TX
	0x4B305458, // K0TX
	0x4C305458, // L0TX
	0x00494144, // aIAD
	0x40494144, // @IAD
	0x01494144, // bIAD
	0x41494144, // AIAD
	0x02494144, // cIAD
	0x42494144, // BIAD
	0x03494144, // dIAD
	0x43494144, // CIAD
	0x04494144, // eIAD
	0x44494144, // DIAD
	0x05494144, // fIAD
	0x45494144, // EIAD
	0x06494144, // gIAD
	0x46494144, // FIAD
	0x07494144, // hIAD
	0x47494144, // GIAD
	0x08494144, // iIAD
	0x48494144, // HIAD
	0x09494144, // jIAD
	0x49494144, // IIAD
	0x0A494144, // kIAD
	0x4A494144, // JIAD
	0x0B494144, // lIAD
	0x4B494144, // KIAD
	0x0C494144, // mIAD
	0x4C494144, // LIAD
	0x0D494144, // nIAD
	0x4D494144, // MIAD
	0x0E494144, // oIAD
	0x4E494144, // NIAD
	0x0F494144, // pIAD
	0x4F494144, // OIAD
	0x10494144, // qIAD
	0x50494144, // PIAD
	0x11494144, // rIAD
	0x51494144, // QIAD
	0x12494144, // sIAD
	0x52494144, // RIAD
	0x13494144, // tIAD
	0x53494144, // SIAD
	0x14494144, // uIAD
	0x54494144, // TIAD
	0x53435054, // SCPT
	0x53434F4C, // SCOL
	0x50574154, // PWAT
	0x434C4443, // CLDC
	0x58585858, // XXXX
	0x5247444C, // RGDL
	0x584C5457, // XLTW
	0x4E565050, // NVPP
	0x58435A41, // XCZA
	0x564E4D4C, // VNML
	0x41545854, // ATXT
	0x56545854, // VTXT
	0x56484754, // VHGT
	0x4D415354 // MAST
};

int main(void) {
    FILE *infile = fopen("SKYRIM.ESM", "rb");
    if (!infile) { perror("Error open"); return 1; }

    fseek(infile, 0, SEEK_END);
    long filesize = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    uint8_t *buffer = malloc(filesize);
    if (!buffer) {
        perror("Not enough memory");
        fclose(infile);
        return 1;
    }

    fread(buffer, 1, filesize, infile);
    fclose(infile);

    uint8_t *out_buffer = malloc(filesize);
    if (!out_buffer) {
        perror("Not enough memory for output");
        free(buffer);
        return 1;
    }

    long i = 0; // cause TES4 is excluded from the while cycle

void twoBytes_nskip(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
    long cur_i = *i;

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two bytes
	out_buffer[cur_i + 5] = buffer[cur_i + 4];

    for (long k = 0; k < *(uint16_t*)&out_buffer[cur_i + 4]; k++) { // and skip
        out_buffer[cur_i + 6 + k] = buffer[cur_i + 6 + k];
    }

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void twoBytes_nextPerFour(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	if (0) {
//	if (*(uint32_t*)&buffer[cur_i] == 0x45504644) { // EPFD
// && (processed_glogal_signature != 0x52454652)

		long length = *(uint16_t*)&out_buffer[cur_i + 4];
//		if ((length > 4) && !(length & 3)) // && (show))
//		if ((length > 5) || (length < 3))
//		if ((length < 4) && (length != 1))
//		if (cur_i < 0x27E0008)
//		if (length == 0x18)
//		if ((*(uint32_t*)&buffer[cur_i + 14] & 0xFFFFFF00) != 0)
			printf("offset 0x%lX %d 0x%4X Glob: 0x%lX\n", cur_i + 4, length, *(uint32_t*)&buffer[cur_i + 6], processed_glogal_signature);

	}
	if (*(uint16_t*)&out_buffer[cur_i + 4] > 0xFF) {
		int known = 0;
		if ((*(uint32_t*)&buffer[cur_i] == 0x424E414D) || // BNAM
			(*(uint32_t*)&buffer[cur_i] == 0x444E414D) || // DNAM
			(*(uint32_t*)&buffer[cur_i] == 0x4A4E414D) || // JNAM
			(*(uint32_t*)&buffer[cur_i] == 0x504E414D) || // PNAM
			(*(uint32_t*)&buffer[cur_i] == 0x524E414D) || // RNAM
			(*(uint32_t*)&buffer[cur_i] == 0x544E414D) || // TNAM
			(*(uint32_t*)&buffer[cur_i] == 0x564E414D) || // VNAM
			(*(uint32_t*)&buffer[cur_i] == 0x4E414D30) || // NAM0
			(*(uint32_t*)&buffer[cur_i] == 0x4D484454) || // MHDT
			(*(uint32_t*)&buffer[cur_i] == 0x4C435352) || // LCSR
			(*(uint32_t*)&buffer[cur_i] == 0x58524744) || // XRGD
			(*(uint32_t*)&buffer[cur_i] == 0x4E565050) || // NVPP
			(*(uint32_t*)&buffer[cur_i] == 0x12494144) || // sIAD
			(*(uint32_t*)&buffer[cur_i] == 0x564D4144) || // VMAD
			(*(uint32_t*)&buffer[cur_i] == 0x4D4F4454) || // MODT
			(*(uint32_t*)&buffer[cur_i] == 0x4E564D49) || // NVMI
			(*(uint32_t*)&buffer[cur_i] == 0x52504C44) || // RPLD
			(*(uint32_t*)&buffer[cur_i] == 0x4C434944) || // LCID
			(*(uint32_t*)&buffer[cur_i] == 0x4C435052) || // LCPR
			(*(uint32_t*)&buffer[cur_i] == 0x4C434550) || // LCEP
			(*(uint32_t*)&buffer[cur_i] == 0x4F465354) || // OFST
			(*(uint32_t*)&buffer[cur_i] == 0x4C43554E) || // LCUN
			(*(uint32_t*)&buffer[cur_i] == 0x52445341) || // RDSA
			(*(uint32_t*)&buffer[cur_i] == 0x44415441)) // DATA
			known = 1;
		if (!known)
			printf("Warning! 0x%lX's length at offset 0x%lX is bigger than 0xFF! It's 0x%1X\n", *(uint32_t*)&buffer[cur_i], cur_i, *(uint16_t*)&out_buffer[cur_i + 4]);
	}
    for (long k = 0; k < *(uint16_t*)&out_buffer[cur_i + 4]; k += 4) {
		out_buffer[cur_i + 6 + k] = buffer[cur_i + 9 + k]; // per four
		out_buffer[cur_i + 7 + k] = buffer[cur_i + 8 + k];
		out_buffer[cur_i + 8 + k] = buffer[cur_i + 7 + k];
		out_buffer[cur_i + 9 + k] = buffer[cur_i + 6 + k];
    }

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void twoBytes_nextPerTwo(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];

    for (long k = 0; k < *(uint16_t*)&out_buffer[cur_i + 4]; k += 2) {
		out_buffer[cur_i + 6 + k] = buffer[cur_i + 7 + k]; // per two
		out_buffer[cur_i + 7 + k] = buffer[cur_i + 6 + k];
    }

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void twoBytes_nextFour_restskip(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;
	
	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two bytes
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 9]; // four
	out_buffer[cur_i + 7] = buffer[cur_i + 8];
	out_buffer[cur_i + 8] = buffer[cur_i + 7];
	out_buffer[cur_i + 9] = buffer[cur_i + 6];
    for (long k = 4; k < *(uint16_t*)&out_buffer[cur_i + 4]; k++) { // and skip
        out_buffer[cur_i + 6 + k] = buffer[cur_i + 6 + k];
    }

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void twoBytes_nextFourPerTwo_restskip(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;
	
	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two bytes
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 7]; // four per two
	out_buffer[cur_i + 7] = buffer[cur_i + 6];
	out_buffer[cur_i + 8] = buffer[cur_i + 9];
	out_buffer[cur_i + 9] = buffer[cur_i + 8];
	out_buffer[cur_i + 10] = buffer[cur_i + 11];
	out_buffer[cur_i + 11] = buffer[cur_i + 10];
	out_buffer[cur_i + 12] = buffer[cur_i + 13];
	out_buffer[cur_i + 13] = buffer[cur_i + 12];
    for (long k = 8; k < *(uint16_t*)&out_buffer[cur_i + 4]; k++) { // and skip
        out_buffer[cur_i + 6 + k] = buffer[cur_i + 6 + k];
    }

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void firstOxlO_nextComplicated(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;
	processed_glogal_signature = *(uint32_t*)&buffer[cur_i];

	out_buffer[cur_i + 4] = buffer[cur_i + 7];
	out_buffer[cur_i + 5] = buffer[cur_i + 6];
	out_buffer[cur_i + 6] = buffer[cur_i + 5];
	out_buffer[cur_i + 7] = buffer[cur_i + 4];
	out_buffer[cur_i + 8] = buffer[cur_i + 11];
	out_buffer[cur_i + 9] = buffer[cur_i + 10];
	out_buffer[cur_i + 10] = buffer[cur_i + 9];
	out_buffer[cur_i + 11] = buffer[cur_i + 8];
	out_buffer[cur_i + 12] = buffer[cur_i + 15];
	out_buffer[cur_i + 13] = buffer[cur_i + 14];
	out_buffer[cur_i + 14] = buffer[cur_i + 13];
	out_buffer[cur_i + 15] = buffer[cur_i + 12];
/*	for (int fu = 0; fu < 124; fu++) {
		if (*(uint32_t*)&buffer[cur_i + 16] == signature_types[fu]) {
			OxC_sized = 1;
			break;
		}
	}*/
//	if (!OxC_sized) {
	if (*(uint32_t*)&buffer[cur_i] != *(uint32_t*)&buffer[cur_i + 16]) {
		out_buffer[cur_i + 16] = buffer[cur_i + 19];
		out_buffer[cur_i + 17] = buffer[cur_i + 18];
		out_buffer[cur_i + 18] = buffer[cur_i + 17];
		out_buffer[cur_i + 19] = buffer[cur_i + 16];
		out_buffer[cur_i + 20] = buffer[cur_i + 21];
		out_buffer[cur_i + 21] = buffer[cur_i + 20];				
		out_buffer[cur_i + 22] = buffer[cur_i + 23];
		out_buffer[cur_i + 23] = buffer[cur_i + 22];
		*i += 24;
	} else {
		*i += 16;
	}
}

void straightFourTeen_Complicated(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;
	if (*(uint32_t*)&buffer[cur_i] != 0x544F4654) // exclude TOFT
		processed_glogal_signature = *(uint32_t*)&buffer[cur_i];
	

	out_buffer[cur_i + 4] = buffer[cur_i + 7]; // 0x10 per 4
	out_buffer[cur_i + 5] = buffer[cur_i + 6];
	out_buffer[cur_i + 6] = buffer[cur_i + 5];
	out_buffer[cur_i + 7] = buffer[cur_i + 4];
	out_buffer[cur_i + 8] = buffer[cur_i + 11];
	out_buffer[cur_i + 9] = buffer[cur_i + 10];
	out_buffer[cur_i + 10] = buffer[cur_i + 9];
	out_buffer[cur_i + 11] = buffer[cur_i + 8];
	out_buffer[cur_i + 12] = buffer[cur_i + 15];
	out_buffer[cur_i + 13] = buffer[cur_i + 14];
	out_buffer[cur_i + 14] = buffer[cur_i + 13];
	out_buffer[cur_i + 15] = buffer[cur_i + 12];
	out_buffer[cur_i + 16] = buffer[cur_i + 19];
	out_buffer[cur_i + 17] = buffer[cur_i + 18];
	out_buffer[cur_i + 18] = buffer[cur_i + 17];
	out_buffer[cur_i + 19] = buffer[cur_i + 16];
	out_buffer[cur_i + 20] = buffer[cur_i + 21]; // 4 per 2
	out_buffer[cur_i + 21] = buffer[cur_i + 20];
	out_buffer[cur_i + 22] = buffer[cur_i + 23];
	out_buffer[cur_i + 23] = buffer[cur_i + 22];
	*i += 24;
}

void global_nskip(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
// Guessed for LAND. Had nothing to precisely compare with. FIXME if that wrong. Also it is used by NAVM, CELL.
// straightFourTeen_Complicated + skip
	long cur_i = *i;
	if(*(uint32_t*)&buffer[cur_i] == 0x43454C4C) // For CELL's DATA
		processed_glogal_signature = 0x43454C4C;

	out_buffer[cur_i + 4] = buffer[cur_i + 7]; // 0x10 per 4
	out_buffer[cur_i + 5] = buffer[cur_i + 6];
	out_buffer[cur_i + 6] = buffer[cur_i + 5];
	out_buffer[cur_i + 7] = buffer[cur_i + 4];
	out_buffer[cur_i + 8] = buffer[cur_i + 11];
	out_buffer[cur_i + 9] = buffer[cur_i + 10];
	out_buffer[cur_i + 10] = buffer[cur_i + 9];
	out_buffer[cur_i + 11] = buffer[cur_i + 8];
	out_buffer[cur_i + 12] = buffer[cur_i + 15];
	out_buffer[cur_i + 13] = buffer[cur_i + 14];
	out_buffer[cur_i + 14] = buffer[cur_i + 13];
	out_buffer[cur_i + 15] = buffer[cur_i + 12];
//  CELL весь при 0 имеет длину 0x10. При всех остальных, по-сути, 0x18 + длина, указанная в первых 4 байтах сразу после "слова" CELL, но бывает, что первые 4 байта не интерпритируются, как длина "чего-то", что должно идти после. Как вариант, проверить на наличие DATA/EDID через 24 байта.
	if ((*(uint32_t*)&buffer[cur_i] != 0x43454C4C) || ((*(uint32_t*)&buffer[cur_i] == 0x43454C4C) && (*(uint32_t*)&out_buffer[cur_i + 4] != 0))) {
		out_buffer[cur_i + 16] = buffer[cur_i + 19];
		out_buffer[cur_i + 17] = buffer[cur_i + 18];
		out_buffer[cur_i + 18] = buffer[cur_i + 17];
		out_buffer[cur_i + 19] = buffer[cur_i + 16];
		out_buffer[cur_i + 20] = buffer[cur_i + 21]; // 2 per 2
		out_buffer[cur_i + 21] = buffer[cur_i + 20];
		out_buffer[cur_i + 22] = buffer[cur_i + 23];
		out_buffer[cur_i + 23] = buffer[cur_i + 22];
		if ((*(uint32_t*)&buffer[cur_i + 24] != 0x44415441) && (*(uint32_t*)&buffer[cur_i + 24] != 0x45444944)) { // Если там дальше ни EDID, ни DATA
			for (long z = 0; z < *(uint32_t*)&out_buffer[cur_i + 4]; z++) { // and skip
				out_buffer[cur_i + 24 + z] = buffer[cur_i + 24 + z];
			}
			*i += 24 + *(uint32_t*)&out_buffer[cur_i + 4];
		} else {
//			long length = *(uint32_t*)&out_buffer[cur_i + 4];
//			if (*(uint32_t*)&buffer[cur_i] == 0x43454C4C)
//				printf("offset 0x%lX %d 0x%1X\n", cur_i + 4, length, *(uint32_t*)&buffer[cur_i + 24]);
			*i += 24;
		}
	} else {
		*i += 16;
	}
}

void XRGD_Special(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
// 4 по 1 пропуск, 6 по 4 свап. Если больше 0x1C, то 4 по 1 пропуск (?). Если ещё больше 0x20, то до конца длины по 4 свап.
	long cur_i = *i;
	long length;

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 6]; // 4 skip
	out_buffer[cur_i + 7] = buffer[cur_i + 7];
	out_buffer[cur_i + 8] = buffer[cur_i + 8];
	out_buffer[cur_i + 9] = buffer[cur_i + 9];
	out_buffer[cur_i + 10] = buffer[cur_i + 13]; // 6 per four
	out_buffer[cur_i + 11] = buffer[cur_i + 12];
	out_buffer[cur_i + 12] = buffer[cur_i + 11];
	out_buffer[cur_i + 13] = buffer[cur_i + 10];
	out_buffer[cur_i + 14] = buffer[cur_i + 17];
	out_buffer[cur_i + 15] = buffer[cur_i + 16];
	out_buffer[cur_i + 16] = buffer[cur_i + 15];
	out_buffer[cur_i + 17] = buffer[cur_i + 14];
	out_buffer[cur_i + 18] = buffer[cur_i + 21];
	out_buffer[cur_i + 19] = buffer[cur_i + 20];
	out_buffer[cur_i + 20] = buffer[cur_i + 19];
	out_buffer[cur_i + 21] = buffer[cur_i + 18];
	out_buffer[cur_i + 22] = buffer[cur_i + 25];
	out_buffer[cur_i + 23] = buffer[cur_i + 24];
	out_buffer[cur_i + 24] = buffer[cur_i + 23];
	out_buffer[cur_i + 25] = buffer[cur_i + 22];
	out_buffer[cur_i + 26] = buffer[cur_i + 29];
	out_buffer[cur_i + 27] = buffer[cur_i + 28];
	out_buffer[cur_i + 28] = buffer[cur_i + 27];
	out_buffer[cur_i + 29] = buffer[cur_i + 26];
	out_buffer[cur_i + 30] = buffer[cur_i + 33];
	out_buffer[cur_i + 31] = buffer[cur_i + 32];
	out_buffer[cur_i + 32] = buffer[cur_i + 31];
	out_buffer[cur_i + 33] = buffer[cur_i + 30];
	
	length = *(uint16_t*)&out_buffer[cur_i + 4];

	if (length > 0x1C) {
		out_buffer[cur_i + 34] = buffer[cur_i + 34]; // more four skip
		out_buffer[cur_i + 35] = buffer[cur_i + 35];
		out_buffer[cur_i + 36] = buffer[cur_i + 36];
		out_buffer[cur_i + 37] = buffer[cur_i + 37];
		if (length > 0x20) {
			for (long k = 0x20; k < *(uint16_t*)&out_buffer[cur_i + 4]; k += 4) {
				out_buffer[cur_i + 6 + k] = buffer[cur_i + 9 + k]; // rest per four
				out_buffer[cur_i + 7 + k] = buffer[cur_i + 8 + k];
				out_buffer[cur_i + 8 + k] = buffer[cur_i + 7 + k];
				out_buffer[cur_i + 9 + k] = buffer[cur_i + 6 + k];
			}

			*i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
		} else {
			*i = cur_i + 38;
		}
    } else {
		*i = cur_i + 34;
	}
}

void GRUP_Special(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
// Есть GRUP, по-сути, стоящие перед самыми первыми объявлениями глобальных переменных (правда, и CELL, видимо, есть такие же не глобальные что-ли...), и в этом случае нужно сделать свап следующих четырёх, и заодно свап по 4 длиной 0x10 всей новой группы. Но есть и другие GRUP, оперирующая как глобальная переменная - тогда и правило ей такое же будет - firstOxlO_nextComplicated
	long cur_i = *i;
	int match;

//	if (*(uint32_t*)&buffer[cur_i + 8] == 0x4E50435F) // GRUP before NPC
//		printf("offset 0x%lX Glob: 0x%lX\n", cur_i, processed_glogal_signature);	


	out_buffer[cur_i + 4] = buffer[cur_i + 7];
	out_buffer[cur_i + 5] = buffer[cur_i + 6];
	out_buffer[cur_i + 6] = buffer[cur_i + 5];
	out_buffer[cur_i + 7] = buffer[cur_i + 4];
	out_buffer[cur_i + 8] = buffer[cur_i + 11];
	out_buffer[cur_i + 9] = buffer[cur_i + 10];
	out_buffer[cur_i + 10] = buffer[cur_i + 9];
	out_buffer[cur_i + 11] = buffer[cur_i + 8];
	out_buffer[cur_i + 12] = buffer[cur_i + 15];
	out_buffer[cur_i + 13] = buffer[cur_i + 14];
	out_buffer[cur_i + 14] = buffer[cur_i + 13];
	out_buffer[cur_i + 15] = buffer[cur_i + 12];
	out_buffer[cur_i + 16] = buffer[cur_i + 19];
	out_buffer[cur_i + 17] = buffer[cur_i + 18];
	out_buffer[cur_i + 18] = buffer[cur_i + 17];
	out_buffer[cur_i + 19] = buffer[cur_i + 16];
	for (long k = 0; k < (long)(sizeof(global_signature_types) >> 2); k++) {
		if (*(uint32_t*)&buffer[cur_i] == global_signature_types[k]) {
			match = 1;
			break;
		}
	}
	if (match) {
		out_buffer[cur_i + 20] = buffer[cur_i + 23];
		out_buffer[cur_i + 21] = buffer[cur_i + 22];
		out_buffer[cur_i + 22] = buffer[cur_i + 21];
		out_buffer[cur_i + 23] = buffer[cur_i + 20];
	} else {
		out_buffer[cur_i + 20] = buffer[cur_i + 21];
		out_buffer[cur_i + 21] = buffer[cur_i + 20];				
		out_buffer[cur_i + 22] = buffer[cur_i + 23];
		out_buffer[cur_i + 23] = buffer[cur_i + 22];
	}
	*i += 24;
}

void XXXX_Special(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
// Кажется как длиной 4, но то четырёхзначное значение означает длину, идующую после следующего "OFST ## ##". Предполагаю, что "## ##" у этого OFST нужно свапить между собой. Ну и сам OFST тоже. Ну а после, на всю указанного значения в XXXX длину свапить по 4.
	long cur_i = *i;

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 9]; // OFST's length inside XXXX
	out_buffer[cur_i + 7] = buffer[cur_i + 8];
	out_buffer[cur_i + 8] = buffer[cur_i + 7];
	out_buffer[cur_i + 9] = buffer[cur_i + 6];
	out_buffer[cur_i + 10] = buffer[cur_i + 13]; // for OFST
	out_buffer[cur_i + 11] = buffer[cur_i + 12];
	out_buffer[cur_i + 12] = buffer[cur_i + 11];
	out_buffer[cur_i + 13] = buffer[cur_i + 10];
	out_buffer[cur_i + 14] = buffer[cur_i + 15]; // OFST's two, (even though they are zeros)
	out_buffer[cur_i + 15] = buffer[cur_i + 14];
    for (long k = 0; k < *(uint32_t*)&out_buffer[cur_i + 6]; k += 4) {
		out_buffer[cur_i + 16 + k] = buffer[cur_i + 19 + k]; // per four
		out_buffer[cur_i + 17 + k] = buffer[cur_i + 18 + k];
		out_buffer[cur_i + 18 + k] = buffer[cur_i + 17 + k];
		out_buffer[cur_i + 19 + k] = buffer[cur_i + 16 + k];
    }

    *i = cur_i + 16 + *(uint32_t*)&out_buffer[cur_i + 6];
}

void VMAD_Special(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
// It's a mess...
	long cur_i = *i;
	int props_quantity;
	long pos, type, tifadd;
	long length, scripts_quantity, fragments_quantity, aliases_quantity;

	void copy_name(long len) {
		for (long z = 0; z < len; z++) {
			out_buffer[cur_i + pos + 2 + z] = buffer[cur_i + pos + 2 + z];
		}
	}

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 7]; // 3 per two
	out_buffer[cur_i + 7] = buffer[cur_i + 6];
	out_buffer[cur_i + 8] = buffer[cur_i + 9];
	out_buffer[cur_i + 9] = buffer[cur_i + 8];
	out_buffer[cur_i + 10] = buffer[cur_i + 11];
	out_buffer[cur_i + 11] = buffer[cur_i + 10];
	pos = 12;
	for (scripts_quantity = *(uint16_t*)&out_buffer[cur_i + 10]; scripts_quantity > 0; scripts_quantity--) { // scripts quantity
		out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two, script's name length
		out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
		length = *(uint16_t*)&out_buffer[cur_i + pos];
		copy_name(length); // skip script name
		pos = pos + 2 + length;
		out_buffer[cur_i + pos] = buffer[cur_i + pos];
		out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
		out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
		props_quantity = *(uint16_t*)&out_buffer[cur_i + pos + 1];
		pos += 3;
		for (; props_quantity > 0; props_quantity--) { // properties quantity
			out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
			out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
			length = *(uint16_t*)&out_buffer[cur_i + pos] + 2;
			copy_name(length); // skip property name
			pos = pos + 2 + length;
			type = *(uint16_t*)&out_buffer[cur_i + pos - 2];
			if (type == 0x010F) { // Array of Bool
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
				length = *(uint32_t*)&out_buffer[cur_i + pos];
				pos += 2;
				copy_name(length); // skip the array itself
				pos = pos + 2 + length;
			} else if ((type == 0x0105) || (type == 0x0305)) { // Bool or Ref's something...
				out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
				pos++;
			} else if ((type == 0x0104) || (type == 0x0103)) { // Float or Int32
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
				pos += 4;
			} else if (type == 0x0102) { // String
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip string name
				pos = pos + 2 + length;
			} else if (type == 0x0101) { // Object
				out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 4 skip
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 3];
				out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 7]; // 1 per four
				out_buffer[cur_i + pos + 5] = buffer[cur_i + pos + 6];
				out_buffer[cur_i + pos + 6] = buffer[cur_i + pos + 5];
				out_buffer[cur_i + pos + 7] = buffer[cur_i + pos + 4];
				pos += 8;
			} else if (type == 0x0301) { // Ref's something...
			// Не смог сравнить, там одни нули были
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two ?? или skip ??
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 5]; // 1 per four
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 4];
				out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 3];
				out_buffer[cur_i + pos + 5] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 6] = buffer[cur_i + pos + 7]; // 1 per two ?? или skip ??
				out_buffer[cur_i + pos + 7] = buffer[cur_i + pos + 6];
				pos += 8;
			} else {
				printf("unknown type 0x%lX at offset 0x%lX glob 0x%lX !\n", type, cur_i + pos - 2, processed_glogal_signature);
			}
		}
	}
	if ((processed_glogal_signature == 0x494E464F) || // INFO
		(processed_glogal_signature == 0x5041434B) || // PACK
		(processed_glogal_signature == 0x5343454E)) { // SCEN
		tifadd = (buffer[cur_i + pos] << 8) + buffer[cur_i + pos + 1];
		if ((tifadd > 0x01FF) && (tifadd < 0x0208)) {
			if ((tifadd == 0x0200) && (processed_glogal_signature == 0x5343454E)) {
				out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 2 skip
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 3]; // 1 per two
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 2];
				length = *(uint16_t*)&out_buffer[cur_i + pos + 2];
				pos += 2;
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
			} else {
				out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 2 skip
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 3]; // 1 per two
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 2];
				length = *(uint16_t*)&out_buffer[cur_i + pos + 2] + 1;
				pos += 2;
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
				if ((tifadd == 0x0203) || (tifadd == 0x0205) || (tifadd == 0x0206) || (tifadd == 0x0207)) {
					out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
					out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2]; // 1 per two
					out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
					length = *(uint16_t*)&out_buffer[cur_i + pos + 1];
					pos++;
					copy_name(length); // skip yet another name...
					pos = pos + 2 + length;
					out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
					out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
					length = *(uint16_t*)&out_buffer[cur_i + pos];
					copy_name(length); // skip yet another name...
					pos = pos + 2 + length;
					if (tifadd == 0x0207) {
						out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
						out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2]; // 1 per two
						out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
						length = *(uint16_t*)&out_buffer[cur_i + pos + 1];
						pos++;
						copy_name(length); // skip yet another name...
						pos = pos + 2 + length;
						out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
						out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
						length = *(uint16_t*)&out_buffer[cur_i + pos];
						copy_name(length); // skip yet another name...
						pos = pos + 2 + length;
					}
				}
			}
			if (processed_glogal_signature == 0x5343454E) { // not over yet for SCEN
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				aliases_quantity = *(uint16_t*)&out_buffer[cur_i + pos];
				pos += 2;
				for (; aliases_quantity > 0; aliases_quantity--) { // not actually 'aliases' quantity...
					out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
					out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 4]; // 1 per four
					out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 3];
					out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 2];
					out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 1];
					out_buffer[cur_i + pos + 5] = buffer[cur_i + pos + 5]; // 1 skip
					pos += 6;
					out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
					out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
					length = *(uint16_t*)&out_buffer[cur_i + pos];
					copy_name(length); // skip yet another name...
					pos = pos + 2 + length;
					out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
					out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
					length = *(uint16_t*)&out_buffer[cur_i + pos];
					copy_name(length); // skip yet another name...
					pos = pos + 2 + length;
				}
			}
		}
	} else if ((processed_glogal_signature == 0x51555354) && ((6 + *(uint16_t*)&out_buffer[cur_i + 4]) != pos)) { // QUST
		out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
		if (buffer[cur_i + pos] != 0) { // fragments
			out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2]; // 1 per two
			out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
			fragments_quantity = *(uint16_t*)&out_buffer[cur_i + pos + 1];
			pos += 3;
			out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
			out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
			length = *(uint16_t*)&out_buffer[cur_i + pos];
			copy_name(length); // skip yet another name...
			pos = pos + 2 + length;
			for (; fragments_quantity > 0; fragments_quantity--) { // fragments quantity
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 2 per four
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
				out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 7];
				out_buffer[cur_i + pos + 5] = buffer[cur_i + pos + 6];
				out_buffer[cur_i + pos + 6] = buffer[cur_i + pos + 5];
				out_buffer[cur_i + pos + 7] = buffer[cur_i + pos + 4];
				out_buffer[cur_i + pos + 8] = buffer[cur_i + pos + 8]; // skip 1
				pos += 9;
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
			}
		} else {pos++;}
		out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
		out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
		aliases_quantity = *(uint16_t*)&out_buffer[cur_i + pos];
		pos += 2;
		for (; aliases_quantity > 0; aliases_quantity--) { // aliases quantity
			out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 2 per two
			out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
			out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 3];
			out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 2];
			out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 7]; // 1 per four
			out_buffer[cur_i + pos + 5] = buffer[cur_i + pos + 6];
			out_buffer[cur_i + pos + 6] = buffer[cur_i + pos + 5];
			out_buffer[cur_i + pos + 7] = buffer[cur_i + pos + 4];
			out_buffer[cur_i + pos + 8] = buffer[cur_i + pos + 9]; // 3 per two
			out_buffer[cur_i + pos + 9] = buffer[cur_i + pos + 8];
			out_buffer[cur_i + pos + 10] = buffer[cur_i + pos + 11];
			out_buffer[cur_i + pos + 11] = buffer[cur_i + pos + 10];
			out_buffer[cur_i + pos + 12] = buffer[cur_i + pos + 13];
			out_buffer[cur_i + pos + 13] = buffer[cur_i + pos + 12];
			pos = pos + 14;
			for (scripts_quantity = *(uint16_t*)&out_buffer[cur_i + pos - 2]; scripts_quantity > 0; scripts_quantity--) { // scripts quantity
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two, script's name length
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip script name
				pos = pos + 2 + length;
				out_buffer[cur_i + pos] = buffer[cur_i + pos];
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
				props_quantity = *(uint16_t*)&out_buffer[cur_i + pos + 1];
				pos += 3;
				for (; props_quantity > 0; props_quantity--) { // properties quantity
					out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
					out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
					length = *(uint16_t*)&out_buffer[cur_i + pos] + 2;
					copy_name(length); // skip property name
					pos = pos + 2 + length;
					type = *(uint16_t*)&out_buffer[cur_i + pos - 2];
					if (type == 0x010F) { // Array of Bool
						out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
						out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
						out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
						out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
						length = *(uint32_t*)&out_buffer[cur_i + pos];
						pos += 2;
						copy_name(length); // skip the array itself
						pos = pos + 2 + length;
					} else if (type == 0x0105) { // Bool
						out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
						pos++;
					} else if ((type == 0x0104) || (type == 0x0103)) { // Float or Int32
						out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
						out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
						out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
						out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
						pos += 4;
					} else if (type == 0x0102) { // String
						out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
						out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
						length = *(uint16_t*)&out_buffer[cur_i + pos];
						copy_name(length); // skip string name
						pos = pos + 2 + length;
					} else if (type == 0x0101) { // Object
						out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 4 skip
						out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 1];
						out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 2];
						out_buffer[cur_i + pos + 3] = buffer[cur_i + pos + 3];
						out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 7]; // 1 per four
						out_buffer[cur_i + pos + 5] = buffer[cur_i + pos + 6];
						out_buffer[cur_i + pos + 6] = buffer[cur_i + pos + 5];
						out_buffer[cur_i + pos + 7] = buffer[cur_i + pos + 4];
						pos += 8;
					} else {
						printf("unknown type 0x%lX at offset 0x%lX glob 0x%lX !\n", type, cur_i + pos - 2, processed_glogal_signature);
					}
				}
			}
		}
	} else if (processed_glogal_signature == 0x5045524B) { // PERK
		out_buffer[cur_i + pos] = buffer[cur_i + pos]; // 1 skip
		if (buffer[cur_i + pos] != 0) { // fragments
			pos++;
			out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
			out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
			length = *(uint16_t*)&out_buffer[cur_i + pos];
			copy_name(length); // skip yet another name...
			pos = pos + 2 + length;
			out_buffer[cur_i + pos] = buffer[cur_i + pos + 1]; // 1 per two
			out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
			fragments_quantity = *(uint16_t*)&out_buffer[cur_i + pos];
			pos += 2;
			for (; fragments_quantity > 0; fragments_quantity--) { // fragments quantity
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
				out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
				out_buffer[cur_i + pos + 4] = buffer[cur_i + pos + 4]; // skip 1
				pos += 5;
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
				out_buffer[cur_i + pos] = buffer[cur_i + pos + 1];
				out_buffer[cur_i + pos + 1] = buffer[cur_i + pos];
				length = *(uint16_t*)&out_buffer[cur_i + pos];
				copy_name(length); // skip yet another name...
				pos = pos + 2 + length;
			}
		}
	}
	if ((6 + *(uint16_t*)&out_buffer[cur_i + 4]) != pos)
		printf("pos %d != %d  offset 0x%lX glob 0x%lX tifadd 0x%lX!\n", pos, 6 + *(uint16_t*)&out_buffer[cur_i + 4], cur_i, processed_glogal_signature, tifadd);

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void NVMI_Special(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
// Только при NAVI
// 6 по 4 свап (NAVM ref, Unknown, 3 coordinates, Preffered Merges Flag)
// 1 по 4 свап, и это есть количество сколько раз делать 1 по 4 свап (Merged To Meshes #)
// 1 по 4 свап, и это есть количество сколько раз делать 1 по 4 свап (Preffered Merges Meshes #)
// 1 по 4 свап, и это есть количество сколько раз делать 2 по 4 свап (Linked Door Refs #)
// 1 так копировать (IsIsland) . Если 1, то:
//+ 6 по 4 свап (Island Data's unknown)
//+ 1 по 4 свап, и это есть количество сколько раз делать 3 по 2 свап (Triangles #)
//+ 1 по 4 свап, и это есть количество сколько раз делать 3 по 4 свап (Vertices #)
// НО если IsIsland 0, то:
//- 1 по 4 свап (Unused)
// 1 по 4 свап (Unknown)
// 1 по 4 свап (Parent WorldSpace)
// 1 по 4 свап (Parent Coordinates)
	long cur_i = *i;
	long pos, pieces;

	void inner_swap_per_four(long pie) {
		for (long z = 0; z < (pie << 2); z += 4) {
			out_buffer[cur_i + pos + 4 + z] = buffer[cur_i + pos + 7 + z]; // per four
			out_buffer[cur_i + pos + 5 + z] = buffer[cur_i + pos + 6 + z];
			out_buffer[cur_i + pos + 6 + z] = buffer[cur_i + pos + 5 + z];
			out_buffer[cur_i + pos + 7 + z] = buffer[cur_i + pos + 4 + z];
		}
	}

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 9]; // 6+1 per four
	out_buffer[cur_i + 7] = buffer[cur_i + 8];
	out_buffer[cur_i + 8] = buffer[cur_i + 7];
	out_buffer[cur_i + 9] = buffer[cur_i + 6];
	out_buffer[cur_i + 10] = buffer[cur_i + 13];
	out_buffer[cur_i + 11] = buffer[cur_i + 12];
	out_buffer[cur_i + 12] = buffer[cur_i + 11];
	out_buffer[cur_i + 13] = buffer[cur_i + 10];
	out_buffer[cur_i + 14] = buffer[cur_i + 17];
	out_buffer[cur_i + 15] = buffer[cur_i + 16];
	out_buffer[cur_i + 16] = buffer[cur_i + 15];
	out_buffer[cur_i + 17] = buffer[cur_i + 14];
	out_buffer[cur_i + 18] = buffer[cur_i + 21];
	out_buffer[cur_i + 19] = buffer[cur_i + 20];
	out_buffer[cur_i + 20] = buffer[cur_i + 19];
	out_buffer[cur_i + 21] = buffer[cur_i + 18];
	out_buffer[cur_i + 22] = buffer[cur_i + 25];
	out_buffer[cur_i + 23] = buffer[cur_i + 24];
	out_buffer[cur_i + 24] = buffer[cur_i + 23];
	out_buffer[cur_i + 25] = buffer[cur_i + 22];
	out_buffer[cur_i + 26] = buffer[cur_i + 29];
	out_buffer[cur_i + 27] = buffer[cur_i + 28];
	out_buffer[cur_i + 28] = buffer[cur_i + 27];
	out_buffer[cur_i + 29] = buffer[cur_i + 26];
	out_buffer[cur_i + 30] = buffer[cur_i + 33];
	out_buffer[cur_i + 31] = buffer[cur_i + 32];
	out_buffer[cur_i + 32] = buffer[cur_i + 31];
	out_buffer[cur_i + 33] = buffer[cur_i + 30];
	pieces = *(uint32_t*)&out_buffer[cur_i + 30]; // Merged To Meshes #
	pos = 30;
	inner_swap_per_four(pieces);
	pos = pos + 4 + (pieces << 2);
	out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
	out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
	out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
	out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
	pieces = *(uint32_t*)&out_buffer[cur_i + pos]; // Preffered Merges Meshes #
	inner_swap_per_four(pieces);
	pos = pos + 4 + (pieces << 2);
	out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
	out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
	out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
	out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
	pieces = (*(uint32_t*)&out_buffer[cur_i + pos]) << 1; // Linked Door Refs #
	inner_swap_per_four(pieces);
	pos = pos + 4 + (pieces << 2);
	out_buffer[cur_i + pos] = buffer[cur_i + pos]; // skip 1, IsIsland
	pos++;
	if (out_buffer[cur_i + pos - 1]) {
		pos = pos - 4;
		pieces = 7; // trick for the further 6+1 per four
		inner_swap_per_four(pieces);
		pos = pos + 32;
		pieces = *(uint32_t*)&out_buffer[cur_i + pos - 4]; // Triangles #
		for (long t = 0; t < (pieces * 6); t += 6) {
			out_buffer[cur_i + pos + t] = buffer[cur_i + pos + 1 + t]; // 3 per two
			out_buffer[cur_i + pos + 1 + t] = buffer[cur_i + pos + t];
			out_buffer[cur_i + pos + 2 + t] = buffer[cur_i + pos + 3 + t];
			out_buffer[cur_i + pos + 3 + t] = buffer[cur_i + pos + 2 + t];
			out_buffer[cur_i + pos + 4 + t] = buffer[cur_i + pos + 5 + t];
			out_buffer[cur_i + pos + 5 + t] = buffer[cur_i + pos + 4 + t];
		}
		pos = pos + (pieces * 6);
		out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // 1 per four
		out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
		out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
		out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
		pieces = 3 * ( *(uint32_t*)&out_buffer[cur_i + pos]); // Vertices #
		inner_swap_per_four(pieces);
		pos = pos + 4 + (pieces << 2);
	} else {
		out_buffer[cur_i + pos] = buffer[cur_i + pos + 3]; // whatever, Unused
		out_buffer[cur_i + pos + 1] = buffer[cur_i + pos + 2];
		out_buffer[cur_i + pos + 2] = buffer[cur_i + pos + 1];
		out_buffer[cur_i + pos + 3] = buffer[cur_i + pos];
		pos = pos + 4;
	}
	pos = pos - 4;
	pieces = 3; // trick for the further 3 per four
	inner_swap_per_four(pieces);
	pos = pos + 16;

    *i = cur_i + 6 + *(uint16_t*)&out_buffer[cur_i + 4];
}

void FourTwoFour_skip(const uint8_t *buffer, uint8_t *out_buffer, long *i) {
	long cur_i = *i;

	out_buffer[cur_i + 4] = buffer[cur_i + 5]; // two
	out_buffer[cur_i + 5] = buffer[cur_i + 4];
	out_buffer[cur_i + 6] = buffer[cur_i + 9]; // four
	out_buffer[cur_i + 7] = buffer[cur_i + 8];
	out_buffer[cur_i + 8] = buffer[cur_i + 7];
	out_buffer[cur_i + 9] = buffer[cur_i + 6];
	*(uint32_t *)(out_buffer + cur_i + 10) = *(const uint32_t *)(buffer + cur_i + 10); // skip 4

	*i += 14;
}

	out_buffer[i]     = buffer[i + 3]; // self for TES4
	out_buffer[i + 1] = buffer[i + 2];
	out_buffer[i + 2] = buffer[i + 1];
	out_buffer[i + 3] = buffer[i];
	straightFourTeen_Complicated(buffer, out_buffer, &i); // TES4, first of all, once

    while (i < filesize) {

			if (i <= filesize - 4) {

			uint32_t current_val = *(uint32_t*)&buffer[i];

			out_buffer[i]     = buffer[i + 3]; // self
			out_buffer[i + 1] = buffer[i + 2];
			out_buffer[i + 2] = buffer[i + 1];
			out_buffer[i + 3] = buffer[i];

			if (current_val == 0x44415441) { // DATA
				if (processed_glogal_signature == 0x434C4153) { // CLAS'es
					// При CLAS только 0x24 - 1 по 4 свап, 20 по 1 так копировать, 2 по 4 свап, 1 по 4 так копировать
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 9]; // 1 per four
					out_buffer[i + 7] = buffer[i + 8];
					out_buffer[i + 8] = buffer[i + 7];
					out_buffer[i + 9] = buffer[i + 6];
					out_buffer[i + 10] = buffer[i + 10]; // 20 skip
					out_buffer[i + 11] = buffer[i + 11];
					out_buffer[i + 12] = buffer[i + 12];
					out_buffer[i + 13] = buffer[i + 13];
					out_buffer[i + 14] = buffer[i + 14];
					out_buffer[i + 15] = buffer[i + 15];
					out_buffer[i + 16] = buffer[i + 16];
					out_buffer[i + 17] = buffer[i + 17];
					out_buffer[i + 18] = buffer[i + 18];
					out_buffer[i + 19] = buffer[i + 19];
					out_buffer[i + 20] = buffer[i + 20];
					out_buffer[i + 21] = buffer[i + 21];
					out_buffer[i + 22] = buffer[i + 22];
					out_buffer[i + 23] = buffer[i + 23];
					out_buffer[i + 24] = buffer[i + 24];
					out_buffer[i + 25] = buffer[i + 25];
					out_buffer[i + 26] = buffer[i + 26];
					out_buffer[i + 27] = buffer[i + 27];
					out_buffer[i + 28] = buffer[i + 28];
					out_buffer[i + 29] = buffer[i + 29];
					out_buffer[i + 30] = buffer[i + 33]; // 2 per four
					out_buffer[i + 31] = buffer[i + 32];
					out_buffer[i + 32] = buffer[i + 31];
					out_buffer[i + 33] = buffer[i + 30];
					out_buffer[i + 34] = buffer[i + 37];
					out_buffer[i + 35] = buffer[i + 36];
					out_buffer[i + 36] = buffer[i + 35];
					out_buffer[i + 37] = buffer[i + 34];
					out_buffer[i + 38] = buffer[i + 38]; // 4 skip
					out_buffer[i + 39] = buffer[i + 39];
					out_buffer[i + 40] = buffer[i + 40];
					out_buffer[i + 41] = buffer[i + 41];
					i += 42;
				} else if (processed_glogal_signature == 0x52414345) { // RACE's
				// При RACE только 0x80 - 14 по 1 так копировать, 1 по 2 ? (там ноль был, не сравнить, пусть будет так копироваться для чётности), 13 по 4 свап, 1 по 4 ? (там были 0xFFFFFFFF, не сравнить, пусть будет свапиться как соседи), 14 по 4 свап
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 6]; // 16 skip
					out_buffer[i + 7] = buffer[i + 7];
					out_buffer[i + 8] = buffer[i + 8];
					out_buffer[i + 9] = buffer[i + 9];
					out_buffer[i + 10] = buffer[i + 10];
					out_buffer[i + 11] = buffer[i + 11];
					out_buffer[i + 12] = buffer[i + 12];
					out_buffer[i + 13] = buffer[i + 13];
					out_buffer[i + 14] = buffer[i + 14];
					out_buffer[i + 15] = buffer[i + 15];
					out_buffer[i + 16] = buffer[i + 16];
					out_buffer[i + 17] = buffer[i + 17];
					out_buffer[i + 18] = buffer[i + 18];
					out_buffer[i + 19] = buffer[i + 19];
					out_buffer[i + 20] = buffer[i + 20];
					out_buffer[i + 21] = buffer[i + 21];
					for (long k = 16; k < *(uint16_t*)&out_buffer[i + 4]; k += 4) {
						out_buffer[i + 6 + k] = buffer[i + 9 + k]; // rest per four
						out_buffer[i + 7 + k] = buffer[i + 8 + k];
						out_buffer[i + 8 + k] = buffer[i + 7 + k];
						out_buffer[i + 9 + k] = buffer[i + 6 + k];
					}

					i += 6 + *(uint16_t*)&out_buffer[i + 4];
				} else if (processed_glogal_signature == 0x47524153) { // GRAS'es
				// При GRAS только 0x20 - 1 по 4 так копировать, 1 по 2 свап, 1 по 2 так копировать, 5 по 4 свап, 4 по 1 так копировать
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 6]; // 4 skip
					out_buffer[i + 7] = buffer[i + 7];
					out_buffer[i + 8] = buffer[i + 8];
					out_buffer[i + 9] = buffer[i + 9];
					out_buffer[i + 10] = buffer[i + 11]; // 2 swap
					out_buffer[i + 11] = buffer[i + 10];
					out_buffer[i + 12] = buffer[i + 12]; // 2 skip
					out_buffer[i + 13] = buffer[i + 13];
					out_buffer[i + 14] = buffer[i + 17]; // 5 per four
					out_buffer[i + 15] = buffer[i + 16];
					out_buffer[i + 16] = buffer[i + 15];
					out_buffer[i + 17] = buffer[i + 14];
					out_buffer[i + 18] = buffer[i + 21];
					out_buffer[i + 19] = buffer[i + 20];
					out_buffer[i + 20] = buffer[i + 19];
					out_buffer[i + 21] = buffer[i + 18];
					out_buffer[i + 22] = buffer[i + 25];
					out_buffer[i + 23] = buffer[i + 24];
					out_buffer[i + 24] = buffer[i + 23];
					out_buffer[i + 25] = buffer[i + 22];
					out_buffer[i + 26] = buffer[i + 29];
					out_buffer[i + 27] = buffer[i + 28];
					out_buffer[i + 28] = buffer[i + 27];
					out_buffer[i + 29] = buffer[i + 26];
					out_buffer[i + 30] = buffer[i + 33];
					out_buffer[i + 31] = buffer[i + 32];
					out_buffer[i + 32] = buffer[i + 31];
					out_buffer[i + 33] = buffer[i + 30];
					out_buffer[i + 34] = buffer[i + 37]; // 4 skip
					out_buffer[i + 35] = buffer[i + 36];
					out_buffer[i + 36] = buffer[i + 35];
					out_buffer[i + 37] = buffer[i + 34];
					i += 38;
				} else if (processed_glogal_signature == 0x57454150) { // WEAP's
				// При WEAP только 0xA - 2 по 4 свап, 1 по 2 свап
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 9]; // 2 per four
					out_buffer[i + 7] = buffer[i + 8];
					out_buffer[i + 8] = buffer[i + 7];
					out_buffer[i + 9] = buffer[i + 6];
					out_buffer[i + 10] = buffer[i + 13];
					out_buffer[i + 11] = buffer[i + 12];
					out_buffer[i + 12] = buffer[i + 11];
					out_buffer[i + 13] = buffer[i + 10];
					out_buffer[i + 14] = buffer[i + 15]; // 1 per two
					out_buffer[i + 15] = buffer[i + 14];
					i += 16;
				} else if (processed_glogal_signature == 0x4449414C) { // DIAL's
					// При DIAL - только 4 - 2 по 1 так копировать, 1 по 2 свап
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 6]; // 2 skip
					out_buffer[i + 7] = buffer[i + 7];
					out_buffer[i + 8] = buffer[i + 9]; // 1 per two
					out_buffer[i + 9] = buffer[i + 8];
					i += 10;
				} else if (processed_glogal_signature == 0x434F4E54) { // CONT's
				// При CONT только 5 - 1 так копировать, 4 по 1 ? (там ноль был, не сравнить, но Tes5Edit кажет, что это float, пусть будет 1 по 4 свап)
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 6]; // 1 skip
					out_buffer[i + 7] = buffer[i + 10]; // 1 per four
					out_buffer[i + 8] = buffer[i + 9];
					out_buffer[i + 9] = buffer[i + 8];
					out_buffer[i + 10] = buffer[i + 7];
					i += 11;
				} else if (processed_glogal_signature == 0x49444C45) { // IDLE's
				// При IDLE только 6 - 4 по 1 так копировать, 1 по 2 свап
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 6]; // 4 skip
					out_buffer[i + 7] = buffer[i + 7];
					out_buffer[i + 8] = buffer[i + 8];
					out_buffer[i + 9] = buffer[i + 9];
					out_buffer[i + 10] = buffer[i + 11]; // 1 per two
					out_buffer[i + 11] = buffer[i + 10];
					i += 12;
				
				} else if (((buffer[i + 4] == 0) && (((buffer[i + 5] < 4) && (buffer[i + 5] != 2)) || ((processed_glogal_signature == 0x5045524B) && (buffer[i + 5] == 5)))) ||// non-PS3's CELL's 1, PERK's 3, 5
				(processed_glogal_signature == 0x57544852) || // WTHR's
				(processed_glogal_signature == 0x43505448) || // CPTH's
				(processed_glogal_signature == 0x44454252) || // DEBR's
				(processed_glogal_signature == 0x45594553) || // EYES'es
				(processed_glogal_signature == 0x48445054) || // HDPT's
				(processed_glogal_signature == 0x4D535454) || // MSTT's
				(processed_glogal_signature == 0x57524C44)) { // WRLD's
				// Значения при PERK:
				//8 - 1 по 4 свап, 4 по 1 так копировать
				// 5 - 1 по 2 ? (там ноль был, не сравнить, пусть будет так копироваться как соседи), 3 копировать как есть
				//4 - свап
				//3 - копировать как есть
				// При WTHR только 0x13 - 15 по 1 так копировать, 2 по 1 ? (там ноль был, не сравнить, пусть будет так копироваться как соседи), 2 по 1 так копировать
				// При DEBR string, так копировать
				// При CPTH, EYES, HDPT, MSTT, WRLD только 1, так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				} else if (processed_glogal_signature == 0x52455642) { // REVB's
				// При REVB только 0xE - 1 по 4 свап, 10 по 1 так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
					out_buffer[i - 14] = buffer[i - 11]; // rewrite 4 swap
					out_buffer[i - 13] = buffer[i - 12];
					out_buffer[i - 12] = buffer[i - 13];
					out_buffer[i - 11] = buffer[i - 14];
				} else if ((processed_glogal_signature == 0x57415452) || // WATR's
				((processed_glogal_signature == 0x43454C4C) && (buffer[i + 4] == 0) && (buffer[i + 5] == 2))) { // CELL's 2
				// При WATR только 2 - 2 по 1 ? ("Damage per second", везде, даже во всех DLC там нули, не сравнить вообще ни с чем, пусть будет свапиться)
				// Значения при CELL - 0x18 - свап
				//2 - 1 по 2 свап
				//4 - ??? у ПК'шных CELL сигнатур DATA не имеется размером 4! На PS3 же там всегда малые значения. Думаю, нужен свап...
					twoBytes_nextPerTwo(buffer, out_buffer, &i);
				} else {
				// При GMST, FACT, ALCH, CSTY, ADDN, FSTP, EQUP, SCEN, ASTP только 4, свап
				// При SCRL, INGR, ARMO, MISC, APPA, KEYM, SLGM только 8, свап
				// При RFCT только 0xC, свап
				// При RELA только 0x10, свап
				// При NAVI, REFR, ACHR, PGRE, PHZD только 0x18, свап
				// При CAMS только 0x28, свап
				// При LIGH, SPGD, MATO только 0x30, свап
				// При EXPL только 0x34, свап
				// При FSTS только кратные 4 не меньше 16 значения, свап
				// При DUAL только 0x18 - 4 по 4 свап, 1 по 4 ? (там были нули, не сравнить, пусть будет свапиться как соседи), 1 по 4 свап
				// При HAZD только 0x26 - 8 по 4 свап, 1 по 4 ? (там были нули, не сравнить, пусть будет свапиться как соседи), 1 по 4 свап
				// При LGTM только 0x5C - 23 по 4 свап, 1 по 4 ? (там были нули, не сравнить, пусть будет свапиться как соседи)
				// При PROJ только 0x5C - 8 по 4 свап, 1 по 4 ? (там были нули, не сравнить, пусть будет свапиться как соседи), 5 по 4 свап, 3 по 4 ? (там были нули, не сравнить, пусть будет свапиться как соседи), 6 по 4 свап

					twoBytes_nextPerFour(buffer, out_buffer, &i);

					if ((((processed_glogal_signature == 0x49504354) || (processed_glogal_signature == 0x5045524B)) && (buffer[i + 5] == 8)) || (processed_glogal_signature == 0x45435A4E)) { // PERK's 8, IPCT's , ECZN's
					// При IPCT только 0x18 - 5 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там ноль был, не сравнить, пусть будет так копироваться для чётности);
					// При ECZN только 0xC - 2 по 4 свап, 4 по 1 так копировать
						out_buffer[i - 4] = buffer[i - 4]; // rewrite 4 skip
						out_buffer[i - 3] = buffer[i - 3];
						out_buffer[i - 2] = buffer[i - 2];
						out_buffer[i - 1] = buffer[i - 1];
					} else if (processed_glogal_signature == 0x45465348) { // EFSH's <----------------- сомнительно
					// При EFSH только 0x0190 - 1 по 4 так копировать, остальные свап
						out_buffer[i - 400] = buffer[i - 400]; // rewrite 4 skip
						out_buffer[i - 399] = buffer[i - 399];
						out_buffer[i - 398] = buffer[i - 398];
						out_buffer[i - 397] = buffer[i - 397];
					} else if (processed_glogal_signature == 0x414D4D4F) { // AMMO's
					// При AMMO только 0x10 - 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там ноль был, не сравнить, пусть будет так копироваться для чётности), 2 по 4 свап
						out_buffer[i - 12] = buffer[i - 12]; // rewrite 4 skip
						out_buffer[i - 11] = buffer[i - 11];
						out_buffer[i - 10] = buffer[i - 10];
						out_buffer[i - 9] = buffer[i - 9];
					} else if (processed_glogal_signature == 0x4D474546) { // MGEF's
					// При MGEF только 0x98 - 5 по 4 свап, 2 по 1 ? (там ноль был, не сравнить, пусть будет так копироваться для кратности), 2 по 1 так копировать, 24 по 4 свап, 2 по 4 (там были нули, не сравнить, пусть будет свапиться как соседи)?, 6 по 4 свап
						out_buffer[i - 132] = buffer[i - 132]; // rewrite 4 skip
						out_buffer[i - 131] = buffer[i - 131];
						out_buffer[i - 130] = buffer[i - 130];
						out_buffer[i - 129] = buffer[i - 129];
					} else if (processed_glogal_signature == 0x424F4F4B) { // BOOK's
					// При BOOK только 0x10 - 2 по 1 так копировать, 2 по 1 ? (там ноль был, не сравнить, пусть будет так копироваться для чётности), 3 по 4 свап
						out_buffer[i - 16] = buffer[i - 16]; // rewrite 4 skip
						out_buffer[i - 15] = buffer[i - 15];
						out_buffer[i - 14] = buffer[i - 14];
						out_buffer[i - 13] = buffer[i - 13];
					}
				}
				continue;
			} else if (current_val == 0x43544441) { // CTDA
				// Самое распространённое, только 0x20, conditions - 4 по 1 ? (не поймал одинаковых значений, первый байт всегда одинаковый, остальные три разные, пусть будут так копироваться), 1 по 4 свап, 1 по 2 свап, 1 по 2 ? (не поймал одинаковых значений, пусть будет свап), 5 по 4 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 6]; // 4 skip
				out_buffer[i + 7] = buffer[i + 7];
				out_buffer[i + 8] = buffer[i + 8];
				out_buffer[i + 9] = buffer[i + 9];
				out_buffer[i + 10] = buffer[i + 13]; // 1 perf four
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				out_buffer[i + 14] = buffer[i + 15]; // 2 per two
				out_buffer[i + 15] = buffer[i + 14];
				out_buffer[i + 16] = buffer[i + 17];
				out_buffer[i + 17] = buffer[i + 16];
				out_buffer[i + 18] = buffer[i + 21]; // 5 per four
				out_buffer[i + 19] = buffer[i + 20];
				out_buffer[i + 20] = buffer[i + 19];
				out_buffer[i + 21] = buffer[i + 18];
				out_buffer[i + 22] = buffer[i + 25];
				out_buffer[i + 23] = buffer[i + 24];
				out_buffer[i + 24] = buffer[i + 23];
				out_buffer[i + 25] = buffer[i + 22];
				out_buffer[i + 26] = buffer[i + 29];
				out_buffer[i + 27] = buffer[i + 28];
				out_buffer[i + 28] = buffer[i + 27];
				out_buffer[i + 29] = buffer[i + 26];
				out_buffer[i + 30] = buffer[i + 33];
				out_buffer[i + 31] = buffer[i + 32];
				out_buffer[i + 32] = buffer[i + 31];
				out_buffer[i + 33] = buffer[i + 30];
				out_buffer[i + 34] = buffer[i + 37];
				out_buffer[i + 35] = buffer[i + 36];
				out_buffer[i + 36] = buffer[i + 35];
				out_buffer[i + 37] = buffer[i + 34];
				i += 38;
				continue;
			} else if (current_val == 0x45444944) { // EDID
				// Самое распространённое, string, название глобального блока, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x46554C4C) { // FULL
				// Самое распространённое, только 4, ссылка на полное название, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44455343) { // DESC
				// Самое распространённое, только 4, ссылка на описание, свап
				// При RACE, SPEL, SCRL, ARMO, BOOK, WEAP, LSCR, PERK, AVIF, MESG, SHOU, COLL известно
				// При CLAS, APPA, AMMO только 4 - 1 по 4 ? (там нули были, не сравнить, пусть свапится как и все другие)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F444C) { // MODL
				// Самое распространённое, только string или 4. В случае string, всё у чего есть 3D моделька, путь до файла.
				if ((processed_glogal_signature == 0x41524D41) || (processed_glogal_signature == 0x41524D4F)) // ARMA's. ARMO's
				// При ARMO, ARMA только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				else
				// При HDPT, RACE, SCRL, ACTI, TACT, BOOK, CONT, DOOR, INGR, LIGH, MISC, STAT, MSTT, GRAS, TREE, FLOR, FURN, WEAP, AMMO, LVLN, KEYM, ALCH, PROJ, HAZD, SLGM, WTHR, CLMT, ANIO, EXPL, BPTD, ADDN, CAMS, IPCT, ARTO, MATO string, так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4F424E44) { // OBND
				// Самое распространённое, только 0xC, 3D координаты, свап по 2
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F4454) { // MODT
				// Самое распространённое.
				// При ACTI, AMMO, PROJ, HAZD, EXPL, ARTO кратные 12 значения, свап
				// При TACT кратные 12 но не совсем совпадающие значения... всё же, свап
				// При HDPT, INGR, MISC, TREE, FLOR, ALCH кратные 12 не меньшие 36 значения, все свап, но 3я четвёрка байт всегда ноль была, не сравнить.
				// При ADDN кратные 12 не меньшие 24 значения, все свап, но 3я четвёрка байт всегда ноль была, не сравнить.
				// При BOOK, CONT, DOOR, STAT, FURN, WEAP, SLGM, DEBR кратные 12 значения, все свап, но 3я четвёрка байт всегда ноль была, не сравнить.
				// При SCRL только 0x24, все свап, но 3я четвёрка байт всегда ноль была, не сравнить.
				// При RACE, LVLN, WTHR, CLMT, ANIO, BPTD, CAMS, MATO только 0xC - 1 по 4 свап, 2 по 4 ? (там нули были, не сравнить, пусть свапится как другие)
				// При LIGH только 0x4C или 0x60, свап
				// При KEYM только 0x3C, свап, но 3я четвёрка байт всегда ноль была, не сравнить.
				// При MSTT, IPCT кратные 4 не меньшие 12 значения, свап
				// При GRAS только 0x24, но не совсем совпадающие значения... всё же, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (((current_val & 0x00FFFFFF) == 0x4E414D) && (((current_val & 0xFF000000) >> 24) > 0x40) && (((current_val & 0xFF000000) >> 24) < 0x5B)) { // #NAM все !!!
				if (current_val == 0x414E414D) { // ANAM
					if ((processed_glogal_signature == 0x52414345) || // RACE's
						(processed_glogal_signature == 0x5041434B) || // PACK's
						(processed_glogal_signature == 0x57415452) || // WATR's
						(processed_glogal_signature == 0x41564946) || // AVIF's
						(processed_glogal_signature == 0x46535450) || // FSTP's
						(processed_glogal_signature == 0x4D555354) || // MUST's
						(processed_glogal_signature == 0x534E4452)) { // SNDR's
					// При RACE, PACK, FSTP, MUST, SNDR string
					// При WATR только 1, так копировать
					// При AVIF только 3, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x5343454E) {// SCEN's
					// Значения при SCEN - 0 пропускать
					//2 - 1 по 2 свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x534F504D) {// SOPM's
					// При SOPM только 0x14 - 4 по 1 ? (нет похожих значений, не с чем сравнить. Я полагаю, тут может так копируется...), 2 по 4 свап, 4 по 1 так копировать, 1 по 4 ? (нет похожих значений, не с чем сравнить. Я полагаю, тут может так копируется...)
						out_buffer[i + 4] = buffer[i + 5]; // two
						out_buffer[i + 5] = buffer[i + 4];
						out_buffer[i + 6] = buffer[i + 6]; // 4 skip
						out_buffer[i + 7] = buffer[i + 7];
						out_buffer[i + 8] = buffer[i + 8];
						out_buffer[i + 9] = buffer[i + 9];
						out_buffer[i + 10] = buffer[i + 13]; // 2 per four
						out_buffer[i + 11] = buffer[i + 12];
						out_buffer[i + 12] = buffer[i + 11];
						out_buffer[i + 13] = buffer[i + 10];
						out_buffer[i + 14] = buffer[i + 17];
						out_buffer[i + 15] = buffer[i + 16];
						out_buffer[i + 16] = buffer[i + 15];
						out_buffer[i + 17] = buffer[i + 14];
						out_buffer[i + 18] = buffer[i + 18]; // 8 skip
						out_buffer[i + 19] = buffer[i + 19];
						out_buffer[i + 20] = buffer[i + 20];
						out_buffer[i + 21] = buffer[i + 21];
						out_buffer[i + 22] = buffer[i + 22];
						out_buffer[i + 23] = buffer[i + 23];
						out_buffer[i + 24] = buffer[i + 24];
						out_buffer[i + 25] = buffer[i + 25];
						i += 26;
					} else {
					// При DOOR, QUST, INFO только 4, свап
					// При IDLE, CPTH только 8, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x424E414D) { // BNAM
					if ((processed_glogal_signature == 0x5041434B) || // PACK's
						(processed_glogal_signature == 0x4D555354) || // MUST's
						(processed_glogal_signature == 0x414E494F)) { // ANIO's
					// При PACK, MUST string
					// При ANIO string, длина 0x12
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x534E4452) { // SNDR's
					// При SNDR только 6 - 4 по 1 так копировать, 1 по 2 свап
						out_buffer[i + 4] = buffer[i + 5]; // two
						out_buffer[i + 5] = buffer[i + 4];
						out_buffer[i + 6] = buffer[i + 6]; // 4 skip
						out_buffer[i + 7] = buffer[i + 7];
						out_buffer[i + 8] = buffer[i + 8];
						out_buffer[i + 9] = buffer[i + 9];
						out_buffer[i + 10] = buffer[i + 11]; // 1 per two
						out_buffer[i + 11] = buffer[i + 10];
						i += 12;
					} else {
					// При ASPC, COBJ, DIAL, MATT, DLVW, COLL только 4, свап
					// При IMAD кратные 4 не меньшие 16 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x434E414D) { // CNAM
					if (((buffer[i + 4] == 0) && (buffer[i + 5] < 4) && (buffer[i + 5] != 2)) || // PACK's 1
						(processed_glogal_signature == 0x54455334) || // TES4's
						(processed_glogal_signature == 0x4449414C)) { // INFO's
					// При DIAL только 1, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При BOOK, WEAP, COBJ, QUST, AVIF, MUST, SNDR, CLFM только 4, свап
					// Значения при PACK - 1, так копировать
					//4 - свап
					// При IMGS только 0xC, свап
					// При MATT только 0xC, свап
					// При TREE только 0x30, свап
					// При COLL только кратные 4 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x444E414D) { // DNAM
					if (processed_glogal_signature == 0x57454150) { // WEAP's
					// При WEAP только 0x64, значения очень плохо сравнивать. Какие-то не совпадали, а при записи в TES5Edit ни на что не влияли, какие-то одни нули или 0xFFFFFFFF... Исправьте "?", если кто-то узнает лучше.
					//1 так копировать, 3 по 1 ? (так копирую),
					//2 по 4 свап,
					//1 по 4 так копировать,
					//1 по 4 свап,
					//1 по 4 ? ("Unknown" там ноль был, не сравнить, пусть будет так копироваться как сосед справа),+
					//4 по 1 так копировать,
					//2 по 4 свап,
					//1 по 4 ? (там ноль был, не сравнить, пусть будет свапиться как соседи),
					//6 по 4 свап,
					//12 по 1 ? (там нули были, не сравнить, пусть будет свапиться как соседи),
					//1 по 4 свап,
					//8 по 1 ? (там нули были, не сравнить, пусть будет свапиться как соседи),
					//4 по 1 ? (там 0xFFFFFFFF были, не сравнить, пусть будет свапиться как соседи),
					//4 по 1 ? (там нули были, не сравнить, пусть будет свапиться как соседи),
					//1 по 4 свап
						out_buffer[i + 4] = buffer[i + 5]; // two
						out_buffer[i + 5] = buffer[i + 4];
						out_buffer[i + 6] = buffer[i + 6]; // 4 skip
						out_buffer[i + 7] = buffer[i + 7];
						out_buffer[i + 8] = buffer[i + 8];
						out_buffer[i + 9] = buffer[i + 9];
						out_buffer[i + 10] = buffer[i + 13]; // 2 per four
						out_buffer[i + 11] = buffer[i + 12];
						out_buffer[i + 12] = buffer[i + 11];
						out_buffer[i + 13] = buffer[i + 10];
						out_buffer[i + 14] = buffer[i + 17];
						out_buffer[i + 15] = buffer[i + 16];
						out_buffer[i + 16] = buffer[i + 15];
						out_buffer[i + 17] = buffer[i + 14];
						out_buffer[i + 18] = buffer[i + 18]; // 4 skip
						out_buffer[i + 19] = buffer[i + 19];
						out_buffer[i + 20] = buffer[i + 20];
						out_buffer[i + 21] = buffer[i + 21];
						out_buffer[i + 22] = buffer[i + 25]; // 1 per four
						out_buffer[i + 23] = buffer[i + 24];
						out_buffer[i + 24] = buffer[i + 23];
						out_buffer[i + 25] = buffer[i + 22];
						out_buffer[i + 26] = buffer[i + 26]; // 8 skip
						out_buffer[i + 26] = buffer[i + 27];
						out_buffer[i + 28] = buffer[i + 28];
						out_buffer[i + 29] = buffer[i + 29];
						out_buffer[i + 30] = buffer[i + 30];
						out_buffer[i + 31] = buffer[i + 31];
						out_buffer[i + 32] = buffer[i + 32];
						out_buffer[i + 33] = buffer[i + 33];
					for (long k = 28; k < *(uint16_t*)&out_buffer[i + 4]; k += 4) {
						out_buffer[i + 6 + k] = buffer[i + 9 + k]; // rest per four
						out_buffer[i + 7 + k] = buffer[i + 8 + k];
						out_buffer[i + 8 + k] = buffer[i + 7 + k];
						out_buffer[i + 9 + k] = buffer[i + 6 + k];
					}

					i += 6 + *(uint16_t*)&out_buffer[i + 4];
					} else if (processed_glogal_signature == 0x51555354) { // QUST's
					// При QUST только 0xC - 1 по 2 свап, 1 по 2 так копировать, 4 по 1 ? ("Unknown", там ноль был, не с чем сравнивать, пусть так копируется, 1 по 4 свап)
						out_buffer[i + 4] = buffer[i + 5]; // two
						out_buffer[i + 5] = buffer[i + 4];
						out_buffer[i + 6] = buffer[i + 7]; // more two
						out_buffer[i + 7] = buffer[i + 6];
						out_buffer[i + 8] = buffer[i + 8]; // 6 skip
						out_buffer[i + 9] = buffer[i + 9];
						out_buffer[i + 10] = buffer[i + 10];
						out_buffer[i + 11] = buffer[i + 11];
						out_buffer[i + 12] = buffer[i + 12];
						out_buffer[i + 13] = buffer[i + 13];
						out_buffer[i + 14] = buffer[i + 17]; // 1 per four
						out_buffer[i + 15] = buffer[i + 16];
						out_buffer[i + 16] = buffer[i + 15];
						out_buffer[i + 17] = buffer[i + 14];
						i += 18;
					} else if (((buffer[i + 4] == 0) && (buffer[i + 5] < 4) && (buffer[i + 5] != 2)) ||
							   (processed_glogal_signature == 0x49444C45) || // IDLE's
							   (processed_glogal_signature == 0x4D41544F) || // MATO's
							   (processed_glogal_signature == 0x4144444E)) { // ADDN's
					// При VTYP, DLVW только 1
					// При IDLE string
					// При MATO своеобразный string. вмещаюший в себя какие-то значения, но всё так копировать как есть
					// При ADDN только 4 - 1 по 2 ? (там ноль был, не с чем сравнивать, пусть так копируется), 2 по 1 так копировать
					// При ARMA 0xC - 8 по 1 так копировать, 1 по 4 свап

						twoBytes_nskip(buffer, out_buffer, &i);

						if (processed_glogal_signature == 0x41524D41) { // ARMA's
							out_buffer[i - 4] = buffer[i - 4]; // rewrite 4 swap
							out_buffer[i - 3] = buffer[i - 3];
							out_buffer[i - 2] = buffer[i - 2];
							out_buffer[i - 1] = buffer[i - 1];
						}
					} else if ((processed_glogal_signature == 0x54585354) || // TXST's
						(processed_glogal_signature == 0x41524D4F)) { // ARMO's
					// При TXST только 2, свап
					// При ARMO только 4 - 1 по 2 свап, 1 по 2 ? (там были нули, не сравнить, пусть будет свапиться как идёт)
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else {
					// При MGEF, INFO, IPCT, MESG, SMBN, SMQN, SMEN, MUST, SCEN, ARTO только 4, свап
					// При RACE, STAT, WRLD только 8, свап
					// При IMGS только 0x10, свап

						twoBytes_nextPerFour(buffer, out_buffer, &i);

						if (processed_glogal_signature == 0x57415452) { // WATR's
						// При WATR только 0xE4 - 7 по 4 свап, 1 по 4 ? (там нули были, не сравнить, пусть будет свапиться как соседи), 5 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там палиндром, не сравнить, пусть так копируется, как сосед слева), 43 по 4 свап
							out_buffer[i - 176] = buffer[i - 176]; // rewrite 4 skip
							out_buffer[i - 175] = buffer[i - 175];
							out_buffer[i - 174] = buffer[i - 174];
							out_buffer[i - 173] = buffer[i - 173];
						} else if (processed_glogal_signature == 0x494D4144) { // IMAD's
						// При IMAD только 0xF4 - 1 по 4 так копировать, 49 по 4 свап, 1 по 4 так копировать, 5 по 4 свап, 1 по 4 так копировать, 4 по 4 свап
							out_buffer[i - 244] = buffer[i - 244]; // rewrite 4 skip
							out_buffer[i - 243] = buffer[i - 243];
							out_buffer[i - 242] = buffer[i - 242];
							out_buffer[i - 241] = buffer[i - 241];
							out_buffer[i - 44] = buffer[i - 44]; // rewrite more 4 skip
							out_buffer[i - 43] = buffer[i - 43];
							out_buffer[i - 42] = buffer[i - 42];
							out_buffer[i - 41] = buffer[i - 41];
							out_buffer[i - 20] = buffer[i - 20]; // rewrite more 4 skip
							out_buffer[i - 19] = buffer[i - 19];
							out_buffer[i - 18] = buffer[i - 18];
							out_buffer[i - 17] = buffer[i - 17];
						} else if (processed_glogal_signature == 0x444F424A) { // DOBJ's
						// При DOBJ одна штука, зато какая, 0x0978. Выглядит как почти абсолютно весь, кроме последних 4 байт, свап. Там где нули не сравнить; есть места, где значения не совпадают, в т.числе последние 4 байта. По моему предположению, 605 по 4 свап, 1 по 2 свап, 1 по 2 ? (там ноль был, не с чем сравнивать, пусть свапится как сосед)
							out_buffer[i - 4] = buffer[i - 3]; // rewrite 2 per two swap
							out_buffer[i - 3] = buffer[i - 4];
							out_buffer[i - 2] = buffer[i - 1];
							out_buffer[i - 1] = buffer[i - 2];
						}
					}
				} else if (current_val == 0x454E414D) { // ENAM
					if ((processed_glogal_signature == 0x51555354) || // QUST's
						(processed_glogal_signature == 0x49444C45)) { // IDLE's
					// При QUST string
					// IDLE string, длина 4
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При FURN, INFO, IPCT, DLVW, SCEN только 4, свап
					// При SMEN только 4, выглядит как string, но нужен свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x464E414D) { // FNAM
					if (((buffer[i + 4] == 0) && (buffer[i + 5] < 4) && (buffer[i + 5] != 2)) ||
						(processed_glogal_signature == 0x434C4D54)) { // CLMT's
					// При RACE только 0, пропускать
					// GLOB, DOOR, REFR, WATR только 1, так копировать
					// При CLMT string, длина 0xC
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if ((processed_glogal_signature == 0x41435449) || // ACTI's
								(processed_glogal_signature == 0x54414354) || // TACT's
								(processed_glogal_signature == 0x464C4F52) || // FLOR's
								(processed_glogal_signature == 0x4655524E)) { // FURN's
					// При ACTI, FURN только 2, свап
					// При TACT, FLOR только 2 - 1 по 2 ? (там нули были, не сравнить, пусть будет свапиться)
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else {
					// При FACT, LIGH, QUST, PACK, AVIF, MATT, LCTN, MUSC, SMQN, SCEN, SNCT, COLL, CLFM только 4, свап
					// При WTHR только 0x20, свап
					// При MUST только кратные 4 не меньше 60 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x474E414D) { // GNAM
					if (processed_glogal_signature == 0x434C4D54) { // CLMT's
					// При CLMT string
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При RACE, LTEX, SNDR, COLL только 4, свап
					// При WATR только 0xC - 3 по 4 ? (там везде нули были, не сравнить, пусть будет свапиться)
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x484E414D) { // HNAM
					if (buffer[i + 5] < 4) {
					// При SCEN только 0, пропуск
					// При LTEX только 2, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При HDPT, AVIF, MATT только 4, свап
					// При IMGS только 0x24, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x494E414D) { // INAM
				// При BOOK, WEAP, REFR, PACK, AVIF, SCEN, ACHR только 4, свап
				// При OTFT только кратные 4 значения, свап
				// При MOVT только 0xC, свап
				// При MESG только 4 - 1 по 4 ? (были нули, но это оставшийся от FallOut механизм иконки к сообщениям, неиспользуемый в TESV, ссылка, свап)
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x4A4E414D) { // JNAM
				// При WTHR только 0x200, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x4B4E414D) { // KNAM
				// При ACTI, FURN, QUST только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x4C4E414D) { // LNAM
				// При RACE, WTHR, REFR, FLST, SCEN, SNDR только 4, свап
				// При MUST только 0xC, свап
				// При INFO только 4 - 1 по 4 ? (на PS3 имеет только одно место, где LNAM внутри INFO, и этого места я не нашёл на Oldrim. Выглядит как ссылка, я считаю, свап)
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x4D4E414D) { // MNAM
					if ((buffer[i + 5] < 4) || 
						(processed_glogal_signature == 0x4D415454) || // MATT's
						(processed_glogal_signature == 0x4D4F5654) || // MOVT's
						(processed_glogal_signature == 0x434F4C4C)) { // COLL's
					// При RACE только 0, пропускать
					// При MATT, MOVT, COLL string
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x57524C44) { // WRLD's
					// При WRLD только 0x10, маловато значений для сравнения - 8 по 1 ? (там нули были, не сравнить, пусть будет свапиться по 2 как соседи справа), 4 по 2 свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else {
					// При FACT, LTEX, FURN, HAZD, WTHR, EXPL, CAMS, LCTN, SMQN, SOPM только 4, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x4E4E414D) { // NNAM
					if ((processed_glogal_signature == 0x57454150) || // WEAP's
						(processed_glogal_signature == 0x57415452)) { // WATR's
					// При WEAP string, длина 0xB
					// При WATR string, длина 0x25
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При WTHR, QUST, LSCR, PERK, SMQN только 4, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x4F4E414D) { // ONAM's
					if ((processed_glogal_signature == 0x4C534352) || // LSCR's
							   (processed_glogal_signature == 0x534F504D)) // SOPM's
					// При LSCR только 4 - 2 по 2 свап
					// При SOPM только 0x18 - 1 по 2 ? (не сравнить, там палиндром, пусть свапится как соседи), 1 по 2 свап, 2 по 2 ? (не сравнить, там палиндром, пусть свапится как соседи), 1 по 2 свап, 1 по 2 ? (не сравнить, там ноль был, пусть свапится как соседи), 3 по 2 свап, 1 по 2 ? (не сравнить, там ноль был, пусть свапится как соседи), 2 по 2 свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					else
					// При REFR только 0, пропускать
					// При RACE, INFO, SNDR только 4, свап
					// При WRLD только 0x10, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x504E414D) { // PNAM
					if ((processed_glogal_signature == 0x464C4F52) || // FLOR's
						((processed_glogal_signature == 0x5041434B) && ((buffer[i + 5] != 4) || (buffer[i + 9] != 1)))) { // PACK' string
					// При FLOR только 4 - 1 по 4 ? ("Unknown", там нули были, не сравнить, пусть так копируется как некоторые другие unknown)
					// При PACK может быть как string, так и флагом, который нужно свапить }:[ Флаг вроде бы как всегда 00 00 00 01, вот на него и проверим, и если не он, то всё так копируем
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x57524C44) { // WRLD's
					// При WRLD только 2, свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else {
					// При HDPT, RACE, ACTI, TACT, FURN, DIAL, AVIF, MATT, LCTN, MUSC, SMBN, SMQN, SMEN, SCEN, SNCT только 4, свап
					// При IPDS только 8, свап
					// При WTHR только 0x200, и всё свап
					// При EQUP только кратные 4 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x514E414D) { // QNAM
					if (processed_glogal_signature == 0x57544852) // WTHR's
					// При WTHR только 0x20, так копировать					
						twoBytes_nskip(buffer, out_buffer, &i);
					else
					// При RACE, CONT, DIAL, PACK, MESG, SMQN, DLBR, DLVW только 4, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x524E414D) { // RNAM
					if (processed_glogal_signature == 0x57544852) {// WTHR's
					// При WTHR 0x20, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x4C534352) {// WTHR's
					// При LSCR только 6 - 3 по 2 свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else {
					// При FACT, HDPT, RACE, ACTI, ARMO, FLOR, INFO, ARMA, LCTN, SMQN только 4, свап
					// При WRLD, IMAD только кратные 4 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x534E414D) { // SNAM
					if ((buffer[i + 5] < 4) ||
						(processed_glogal_signature == 0x54455334)) { // TES4's
					// При LTEX только 1, так копировать
					// При TES4 string, комментарий к плагину, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При ASPC, ACTI, CONT, DOOR, LIGH, MSTT, TREE, FLOR, WEAP, LSCR, WATR, AVIF, CPTH, IPCT, SMBN, SMQN, SMEN, DLBR, SCEN, SNDR только 4, свап
					// При WTHR только 8, свап
					// При SHOU только 0xC, свап
					// При INFO только 4, но подобных секций нет в ПК'шной версии, не сравнимо. Выглядит как ссылка, я считаю, свап.
					// При DIAL только 4, выглядит как string, но нужен свап
					// При IMAD только кратные 4 не меньшие 16 значения, свап
					// При MUST только кратные 4 не меньшие 52 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x544E414D) { // TNAM
					if (processed_glogal_signature == 0x434C4D54) {// CLMT's
					// При CLMT только 6, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if (processed_glogal_signature == 0x52454652) {// REFR's
					// При REFR только 2, свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					} else {
					// При HDPT, LTEX, ARMO, WEAP, WTHR, WATR, MESG, DLBR, DLVW, WOOP только 4, свап
					// При IMGS только 0x10, свап
					// При IMAD только кратные 20 значения, свап
					// При MUSC только кратные 4 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x554E414D) { // UNAM
					if (buffer[i + 5] < 4) {
						if (buffer[i + 5] == 2)
						// При SNCT только 2, свап
							twoBytes_nextPerTwo(buffer, out_buffer, &i);
						else
						// При PACK только 1, так копировать
							twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При RACE, WEAP только 4, свап
					// При IMAD только кратные не меньшие 16 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x564E414D) { // VNAM
					if (buffer[i + 5] == 2)
					// При SNCT только 2, свап
						twoBytes_nextPerTwo(buffer, out_buffer, &i);
					else
					// При RACE, ACTI, TACT, WEAP, PROJ, AVIF только 4, свап
					// При SCEN только 0x10, свап
					// При IMAD только кратные не меньшие 16 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x574E414D) { // WNAM
				// При RACE, ACTI, WEAP, REGN, WRLD, MUSC, SCEN только 4, свап
				// При IMAD только кратные 8 не меньшие 16 значения, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x584E414D) { // XNAM
					if (buffer[i + 5] < 4) {
					// При PACK только 1, так копировать
						twoBytes_nskip(buffer, out_buffer, &i);
					} else if ((processed_glogal_signature == 0x534D424E) || // SMBN's
							   (processed_glogal_signature == 0x534D454E)) { // SMEN's
					// При SMBN, SMEN только 4 - 1 по 4 ? ("Unknown", там нули были, не сравнить, пусть так копируется как некоторые другие unknown)
						twoBytes_nskip(buffer, out_buffer, &i);
					} else {
					// При AVIF, SMQN только 4, свап
					// При FACT, LSCR только 0xC, свап
					// При IMAD только кратные 8 не меньшие 16 значения, свап
						twoBytes_nextPerFour(buffer, out_buffer, &i);
					}
				} else if (current_val == 0x594E414D) { // YNAM
				// При ARMO, BOOK, INGR, MISC, AMMO, KEYM, ALCH, AVIF только 4, свап
				// При IMAD только кратные 8 не меньшие 16 значения, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				} else if (current_val == 0x5A4E414D) { // ZNAM
				// При ARMO, INGR, MISC, AMMO, KEYM, ALCH, WRLD только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				}
				continue;
			} else if (current_val == 0x4E414D45) { // NAME
				if (processed_glogal_signature == 0x52414345) // RACE's
				// Значения при RACE - 1 или string, всё так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				else
				// При REFR, ACHR, PHZD, PGRE только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D30) { // NAM0
				if (processed_glogal_signature == 0x5343454E) // SCEN's
				// Значения при SCEN - 1 или string, всё так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				// При RACE только 0, пропускать
				// При HDPT, FURN, SLGM, QUST, ARMA, LCTN только 4, свап
				// При WRKD только 8, свап
				// При WATR только 0xC, свап
				// При WTHR только 0x0110, свап
				else
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D31) { // NAM1
				if ((processed_glogal_signature == 0x48445054) || // HDPT's
					(processed_glogal_signature == 0x50524F4A) || // PROJ's
					(processed_glogal_signature == 0x42505444)) { // BTPD's
				// Значения при PROJ - 1 или string, всё так копировать
				// При HDPT string
				// При BTPD только 1, так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				} else if (processed_glogal_signature == 0x434F424A) { // COBJ's
				// При COBJ только 2, свап
					twoBytes_nextPerTwo(buffer, out_buffer, &i);
				} else {
				// При RACE только 0, пропускать
				// При WTHR, IPCT, ARMA, LCTN, SOPM только 4, свап
				// При WATR только 0xC, свап
				// При IMAD только кратные 8 большие 16 значения, свап
				// При INFO только 4 - 1 по 4 ? (видимо, находятся в секциях INFO, которые есть только на PS3. Выглядят как float, я считаю, свап)
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				}
				continue;
			} else if (current_val == 0x4E414D32) { // NAM2
				// При WRLD, IPCT, ARMA только 4, свап
				// При IMAD только кратные 8 большие 16 значения, свап
				// При PROJ только кратные 4 не меньшие 12 значения. Конкретно вторую четвёрку байт не сравнить, там были нули, но все остальные свапятся, значит пусть и те нули свапятся.
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D33) { // NAM3
					// При RACE только 0, пропускать
					// При WRLD, ARMA только 4, свап
					// При IMAD только кратные 20 значения, свап					
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D34) { // NAM4
				if (processed_glogal_signature == 0x42505444) // BTPD's
				// Значения при BTPD - 2 или string, всё так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				else
				// При RACE, WRLD только 4, свап
				// При IMAD только 0x10, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D35) { // NAM5
				// При BTPD только 0, пропускать
				// При RACE только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
// NAM6 нету
			} else if (current_val == 0x4E414D37) { // NAM7
				if (processed_glogal_signature == 0x45465348) // EFSH's
				// Значения при EFSH - 1 или string, всё так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				else
				// При RACE только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D38) { // NAM8
				if (processed_glogal_signature == 0x45465348) // EFSH's
				// Значения при EFSH - 1 или string, всё так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				else
				// При RACE, WEAP только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D39) { // NAM9
				if (processed_glogal_signature == 0x45465348) // EFSH's
				// Значения при EFSH - 1 или string, всё так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				else
				// При WEAP только 4, свап
				// При WRLD только 8, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C564C4F) { // LVLO
			// Довольно распространённое.
			// При LVLN, LVLI, LVSP только 0xC - 1 по 2 свап, 1 по 2 ? (там нули везде были, не сравнить, пусть свапятся как сосед слева), 1 по 4 свап, 1 по 2 свап, 1 по 2 ? (там нули везде были, не сравнить, пусть свапятся как сосед слева)
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				out_buffer[i - 8] = buffer[i - 5]; // rewrite 4 swap
				out_buffer[i - 7] = buffer[i - 6];
				out_buffer[i - 6] = buffer[i - 7];
				out_buffer[i - 5] = buffer[i - 8];
				continue;
			} else if (current_val == 0x4D4F3253) { // MO2S
				// При ARMO string содержащие значения - 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи), до четырёх байт до конца так копировать, последние 1 по 4 ? (там нули были, не сравнить, пусть так копируется как соседи)
				// При ARMA string содержащие значения - 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи), до двух байт до конца так копировать, последние 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи)
				twoBytes_nextFour_restskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F3353) { // MO3S
				// String содержащие значения - 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи), до двух байт до конца так копировать, последние 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи)
				twoBytes_nextFour_restskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F3453) { // MO4S
				// При ARMO string содержащие значения - 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи), до четырёх байт до конца так копировать, последние 1 по 4 ? (там нули были, не сравнить, пусть так копируется как соседи)
				// При ARMA string содержащие значения - 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи), до двух байт до конца так копировать, последние 1 по 2 ? (там нули были, не сравнить, пусть так копируется как соседи)
				twoBytes_nextFour_restskip(buffer, out_buffer, &i);
				continue;
// Не нашлось в Skyrim.esm, но предположу, что у него структура та же
//			} else if (current_val == 0x4D4F3553) { // MO5S
//					twoBytes_nextFour_restskip(buffer, out_buffer, &i);
//				continue;
			} else if ((current_val > 0x5458302F) && (current_val < 0x54583038)) { // TX00-TX07
				// Абслютно все как string, копируем как есть
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (((current_val & 0x00FFFFFF) == 0x305458) && ((current_val >> 24) > 0x2F) && ((current_val >> 24) < 0x4D)){ // Все Ɣ0TX
				// Абслютно все как string, копируем как есть
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if ((current_val > 0x4D4F4431) && (current_val < 0x4D4F4436)) { // MOD2-MOD5
				// Абслютно все как string, копируем как есть
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F3254) { // MO2T
				// При ARMO, ARMA кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F3354) { // MO3T
				// Только при ARMA кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F3454) { // MO4T
				// При ARMO кратные 12 значения, свап
				// При ARMA только 0xC, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F3554) { // MO5T
				// При ARMA только 0xC - 1 по 4 свап, 2 по 4 ? (там нули были, не сравнить, пусть свапится как в других MOxT)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
// MO5S, BMCT, TDVT, XCLN, XILL, XWEM, XCCM, XORD, XPTL, XCZR, XCHG, XLRL, XATR, XLOD, ACPR, RCPR, ACUN, RCUN, ACSR, RCSR, ACID, ACEP, ACBS, TPLT, ATKR, OCOR, GWOR, PRKZ, PRKR, AIDT, DOFT, SOFT, DPLT, CRIF, FTST, TIAS, SPMV, TX06  не нашлись в Skyrim.esm
// XCLL, XWCS, XCWT, XOCP, XMBP, CSMD, TPIC - нет в PS3'шном Skyrim.esm! Но они есть в ПК'шном!
			} else if (current_val == 0x474D5354) { // GMST

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41414354) { // AACT

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41434852) { // ACHR !!! check!

				straightFourTeen_Complicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41435449) { // ACTI

				firstOxlO_nextComplicated(buffer, out_buffer, &i);
//				for (int fu = 0; fu < 124; fu++) {
	//				if (*(uint32_t*)&buffer[i] == signature_types[fu])
		//				printf("Check ACTI at offset 0x%lX\n", i);
			//	}
				continue;
			} else if (current_val == 0x4144444E) { // ADDN !!!check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x414C4348) { // ALCH !!!check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x414D4D4F) { // AMMO !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x414E494F) { // ANIO !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41505041) { // APPA !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41524D41) { // ARMA !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41524D4F) { // ARMO !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4152544F) { // ARTO !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41535043) { // ASPC !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41564946) { // AVIF !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x424F4F4B) { // BOOK !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x42505444) { // BPTD !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x43414D53) { // CAMS !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x43454C4C) { // CELL !!! check!

				global_nskip(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x434C4153) { // CLAS !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x434C464D) { // CLFM !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x434C4D54) { // CLMT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x434F424A) { // COBJ !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x434F4C4C) { // COLL !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x434F4E54) { // CONT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x43505448) { // CPTH !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x43535459) { // CSTY !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x44454252) { // DEBR !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4449414C) { // DIAL !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x444C4252) { // DLBR !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x444C5657) { // DLVW !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x444F4F52) { // DOOR !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x45435A4E) { // ECZN !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x45465348) { // EFSH !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x454E4348) { // ENCH !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4558504C) { // EXPL !!! check!!! Где-то что-то внутри полей или имён содержит EXPL!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x46414354) { // FACT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x464C4F52) { // FLOR !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x464C5354) { // FLST !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x46535450) { // FSTP !!! check!!! Где-то что-то внутри полей или имён содержит FSTP!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x46535453) { // FSTS !!! check!!! Где-то что-то внутри полей или имён содержит FSTS!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4655524E) { // FURN !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x474C4F42) { // GLOB !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x47524153) { // GRAS !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x47525550) { // GRUP

				GRUP_Special(buffer, out_buffer, &i);

				continue;
// HAIR is only one in Skyrim.esm. Length 0x10. It might be it needs
// firstOxlO_nextComplicated
// Swapping by the GRUP algorhitm.
			} else if (current_val == 0x48415A44) { // HAZD !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x48445054) { // HDPT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x49444C45) { // IDLE !!! check!!! Где-то что-то внутри полей или имён содержит IDLE!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x49444C4D) { // IDLM !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x494D4144) { // IMAD !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x494D4753) { // IMGS !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x494E464F) { // INFO !!! check!

				straightFourTeen_Complicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x494E4752) { // INGR !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x49504354) { // IPCT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x49504453) { // IPDS !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4B45594D) { // KEYM !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4B595744) { // KYWD !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C414E44) { // LAND !!! check!

				global_nskip(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C435254) { // LCRT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C43544E) { // LCTN !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C47544D) { // LGTM !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C494748) { // LIGH !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C534352) { // LSCR !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C544558) { // LTEX !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C564C49) { // LVLI !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C564C4E) { // LVLN !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4C565350) { // LVSP !!! check!!! Где-то что-то внутри полей или имён содержит LVSP!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D41544F) { // MATO !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D415454) { // MATT !!! check!!! Где-то что-то внутри полей или имён содержит MATT!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D455347) { // MESG !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D474546) { // MGEF !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D495343) { // MISC !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D4F5654) { // MOVT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D535454) { // MSTT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D555343) { // MUSC !!! check!!! Где-то что-то внутри полей или имён содержит MUSC!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D555354) { // MUST !!! check!!! Где-то что-то внутри полей или имён содержит MUST!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x58524742) { // XRGB
				// При ACHR, REFR только 0xC, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504B4332) { // PKC2
				// Только при PACK и только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58495332) { // XIS2
				// При REFR, ACHR только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43495331) { // CIS1
				// При IDLE, PACK, CPTH string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C435352) { // LCSR
				// При LCTN большие кратные 16 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58524744) { // XRGD
				XRGD_Special(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50464F32) { // PFO2
				// Только при PACK и только 0x10 - 2 по 4 свап, 2 по 2 свап, 4 по 1 ? (не сравнимые не одинаковые значения, но не похоже чтобы что-то свапалось, я считаю, так копировать)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 2 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 13]; 
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				out_buffer[i + 14] = buffer[i + 15]; // 2 per two
				out_buffer[i + 15] = buffer[i + 14];
				out_buffer[i + 16] = buffer[i + 17];
				out_buffer[i + 17] = buffer[i + 16];
				out_buffer[i + 18] = buffer[i + 18]; // 4 skip
				out_buffer[i + 19] = buffer[i + 19];
				out_buffer[i + 20] = buffer[i + 20];
				out_buffer[i + 21] = buffer[i + 21];
				i += 22;
				continue;
			} else if (current_val == 0x494E5456) { // INTV
				// При TES4 4, очевидно, одна штука свап
				// При COLL только 4 - 1 по 4 ? (там нули были, не сравнить, пусть свапится, как оно показало себя в TES4)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52444154) { // RDAT
				// При ASPC только 4, свап
				// При REGN только 8 - 1 по 4 свап, 2 по 1 так копировать, 2 по 1 ? (там нули были, не сравнить, пусть так копируется как и копируется)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 1 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				if (processed_glogal_signature == 0x5245474E) { // REGN's
					out_buffer[i + 10] = buffer[i + 10]; // 4 skip
					out_buffer[i + 11] = buffer[i + 11];
					out_buffer[i + 12] = buffer[i + 12];
					out_buffer[i + 13] = buffer[i + 13];
					i += 14;
				} else {
					i += 10;
				}
				continue;
			} else if (current_val == 0x464C5456) { // FLTV
				// При GLOB, MUST только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x544F4654) { // TOFT !!! Эксклюзивно не на ПК

				straightFourTeen_Complicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x54494643) { // TIFC
				// Только при DIAL и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54524454) { // TRDT
				// Только при INFO и только 0x18 - 2 по 4 свап, 1 по 4 ? (там везде нули были, не сравнить... даже не знаю... тут и свап, и не свап... я выбрал свап наобум), 4 по 1 так копировать, 1 по 4 свап, 4 по 1 так копировать
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 2 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 13];
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				out_buffer[i + 14] = buffer[i + 17]; // 1 per four ¿questionable? swap
				out_buffer[i + 15] = buffer[i + 16];
				out_buffer[i + 16] = buffer[i + 15];
				out_buffer[i + 17] = buffer[i + 14];
				out_buffer[i + 18] = buffer[i + 18]; // 4 skip
				out_buffer[i + 19] = buffer[i + 19];
				out_buffer[i + 20] = buffer[i + 20];
				out_buffer[i + 21] = buffer[i + 21];
				out_buffer[i + 22] = buffer[i + 25]; // 1 per four
				out_buffer[i + 23] = buffer[i + 24];
				out_buffer[i + 24] = buffer[i + 23];
				out_buffer[i + 25] = buffer[i + 22];
				out_buffer[i + 26] = buffer[i + 26]; // 4 skip
				out_buffer[i + 27] = buffer[i + 27];
				out_buffer[i + 28] = buffer[i + 28];
				out_buffer[i + 29] = buffer[i + 29];
				i += 30;
				continue;
			} else if (current_val == 0x564D4144) { // VMAD

				VMAD_Special(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x554E4553) { // UNES
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4B53495A) { // KSIZ
			// Только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D544E4D) { // MTNM
			// Только при RACE и только 4, выглядит как string, но нужен свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5654434B) { // VTCK
			// Только при RACE и только 8, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x48434C46) { // HCLF
			// Только при RACE и только 8, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E4C) { // TINL !! Только в не консольной версии !!
			// Только при RACE и только 2 - 1 по 2 ? (не сравнить раз на PS3 нет. Больше похоже на двухбайтовое число, нежели на два по байту, я считаю, свап)
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x41544B44) { // ATKD
			// Только при RACE и только 0x2C, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x41544B45) { // ATKE
			// Только при RACE, string
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x494E4458) { // INDX
				if (processed_glogal_signature == 0x51555354) { // QUST's
				// При QUST только 4 - 1 по 2 свап, 2 по 1 так копировать
					out_buffer[i + 4] = buffer[i + 5]; // two
					out_buffer[i + 5] = buffer[i + 4];
					out_buffer[i + 6] = buffer[i + 7]; // 1 per two
					out_buffer[i + 7] = buffer[i + 6];
					out_buffer[i + 8] = buffer[i + 8]; // 2 skip
					out_buffer[i + 9] = buffer[i + 9];
					i += 10;
				} else {
				// При RACE только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				}
				continue;
			} else if (current_val == 0x50485754) { // PHWT
			// Только при RACE, 0x20 или 0x40, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D504149) { // MPAI
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D504156) { // MPAV
			// Только при RACE и только 0x20 - 2 по 4 свап, 6 по 4 ? (там 0xFFFFFFFF были, не сравнить, я считаю, свап)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4148434D) { // AHCM
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E49) { // TINI
			// Только при RACE и только 2, свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E43) { // TINC
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E54) { // TINT
			// Только при RACE, string
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E50) { // TINP
			// Только при RACE и только 2, свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E56) { // TINV
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54495253) { // TIRS
			// Только при RACE и только 2, свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x41484346) { // AHCF
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54494E44) { // TIND
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53504C4F) { // SPLO
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x424F4454) { // BODT
			// При RACE, ARMO, ARMA только 0xC - 1 по 4 свап, 1 по 4 так копировать, 1 по 4 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 1 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 10]; // 4 skip
				out_buffer[i + 11] = buffer[i + 11];
				out_buffer[i + 12] = buffer[i + 12];
				out_buffer[i + 13] = buffer[i + 13];
				out_buffer[i + 14] = buffer[i + 17]; // 1 per four
				out_buffer[i + 15] = buffer[i + 16];
				out_buffer[i + 16] = buffer[i + 15];
				out_buffer[i + 17] = buffer[i + 14];
				i += 18;
				continue;
			} else if (current_val == 0x43525641) { // CRVA
			// Только при FACT и только 0x14 - 5 по 2 свап, 1 по 2 так копировать, 1 по 4 свап, 2 по 2 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 7]; // 5 per two
				out_buffer[i + 7] = buffer[i + 6];
				out_buffer[i + 8] = buffer[i + 9];
				out_buffer[i + 9] = buffer[i + 8];
				out_buffer[i + 10] = buffer[i + 11];
				out_buffer[i + 11] = buffer[i + 10];
				out_buffer[i + 12] = buffer[i + 13];
				out_buffer[i + 13] = buffer[i + 12];
				out_buffer[i + 14] = buffer[i + 15];
				out_buffer[i + 15] = buffer[i + 14];
				out_buffer[i + 16] = buffer[i + 16]; // 2 skip
				out_buffer[i + 17] = buffer[i + 17];
				out_buffer[i + 18] = buffer[i + 21]; // 1 per four
				out_buffer[i + 19] = buffer[i + 20];
				out_buffer[i + 20] = buffer[i + 19];
				out_buffer[i + 21] = buffer[i + 18];
				out_buffer[i + 22] = buffer[i + 23]; // 2 per two
				out_buffer[i + 23] = buffer[i + 22];
				out_buffer[i + 24] = buffer[i + 25];
				out_buffer[i + 25] = buffer[i + 24];
				i += 26;
				continue;
			} else if (current_val == 0x444D494E) { // DMIN
			// Только при SCEN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x444D4158) { // DMAX
			// Только при SCEN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x48544944) { // HTID
			// Только при SCEN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44455641) { // DEVA
			// Только при SCEN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4944) { // ALID
				if (processed_glogal_signature == 0x51555354) // QUST's
				// При QUST string, так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				else
				// При SCEN только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50524B45) { // PRKE
			// Только при PERK и только 3, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504B4454) { // PKDT
			// Только при PACK и только 0xC - 1 по 4 свап, 1 по 4 так копировать, 2 по 2 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 1 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 10]; // skip 4
				out_buffer[i + 11] = buffer[i + 11];
				out_buffer[i + 12] = buffer[i + 12];
				out_buffer[i + 13] = buffer[i + 13];
				out_buffer[i + 14] = buffer[i + 15]; // 2 per two
				out_buffer[i + 15] = buffer[i + 14];
				out_buffer[i + 16] = buffer[i + 17];
				out_buffer[i + 17] = buffer[i + 16];
				i += 18;
				continue;
			} else if (current_val == 0x504B4355) { // PKCU
			// Только при PACK и только 0xC, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504C4454) { // PLDT
			// Только при PACK и только 0xC - 1 так копировать, 3 по 1 ? (там нули были, не сравнить, я выбрал так копировать), 2 по 4 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 6]; // skip 4
				out_buffer[i + 7] = buffer[i + 7];
				out_buffer[i + 8] = buffer[i + 8];
				out_buffer[i + 9] = buffer[i + 9];
				out_buffer[i + 10] = buffer[i + 13]; // 2 per four
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				out_buffer[i + 14] = buffer[i + 17];
				out_buffer[i + 15] = buffer[i + 16];
				out_buffer[i + 16] = buffer[i + 15];
				out_buffer[i + 17] = buffer[i + 14];
				i += 18;
				continue;
			} else if (current_val == 0x5044544F) { // PDTO
			// При PACK только 8 - 1 так копировать, 3 по 1 ? (там нули были, не сравнить, я выбрал так копировать), 1 по 4 свап
			// При REFR, ACHR только 8 - 4 по 1 ? (там нули были, не сравнить, я выбрал так копировать, как при PACK), 1 по 4 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 6]; // skip 4
				out_buffer[i + 7] = buffer[i + 7];
				out_buffer[i + 8] = buffer[i + 8];
				out_buffer[i + 9] = buffer[i + 9];
				out_buffer[i + 10] = buffer[i + 13]; // 1 per four
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				i += 14;
				continue;
			} else if (current_val == 0x504F4341) { // POCA
			// Только при PACK и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504F4541) { // POEA
			// Только при PACK и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504F4241) { // POBA
			// Только при PACK и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50534454) { // PSDT
			// Только при PACK и только 0xC - 5 по 1 так копировать, 3 по 1 ? (значения не совпадали, не сравнить, я выбрал так копировать), 1 по 4 свап
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 6]; // skip 8
				out_buffer[i + 7] = buffer[i + 7];
				out_buffer[i + 8] = buffer[i + 8];
				out_buffer[i + 9] = buffer[i + 9];
				out_buffer[i + 10] = buffer[i + 10];
				out_buffer[i + 11] = buffer[i + 11];
				out_buffer[i + 12] = buffer[i + 12];
				out_buffer[i + 13] = buffer[i + 13];
				out_buffer[i + 14] = buffer[i + 17]; // 1 per four
				out_buffer[i + 15] = buffer[i + 16];
				out_buffer[i + 16] = buffer[i + 15];
				out_buffer[i + 17] = buffer[i + 14];
				i += 18;
				continue;
			} else if (current_val == 0x43495332) { // CIS2
			// При FACT, ENCH, SPEL, INFO, QUST, PACK, LSCR, PERK, SMBN, SMQN, SCEN string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4549544D) { // EITM
			// При ARMO, WEAP, EXPL только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x434E544F) { // CNTO
			// При CONT, COBJ, QUST только 8, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x434F4354) { // COCT
			// При CONT, COBJ, QUST только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x56454E56) { // VENV
			// Только при FACT и только 0xC - 2 по 2 свап, 1 по 4 свап, 1 по 2 так копировать, 1 по 2 ? (там нули были, не сравнить, пусть так копируется как сосед)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 7]; // 2 per two
				out_buffer[i + 7] = buffer[i + 6];
				out_buffer[i + 8] = buffer[i + 9];
				out_buffer[i + 9] = buffer[i + 8];
				out_buffer[i + 10] = buffer[i + 13]; // 1 per four
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				out_buffer[i + 14] = buffer[i + 14]; // skip 4
				out_buffer[i + 15] = buffer[i + 15];
				out_buffer[i + 16] = buffer[i + 16];
				out_buffer[i + 17] = buffer[i + 17];
				i += 18;
				continue;
			} else if (current_val == 0x58505041) { // XPPA
			// Только при REFR и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5850524D) { // XPRM
			// Только при REFR и только 0x20, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584D424F) { // XMBO
			// Только при REFR и только 0xC, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58505244) { // XPRD
			// При REFR, ACHR только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C524D) { // XLRM
			// Только при REFR и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584E4450) { // XNDP
			// Только при REFR и только 8 - 1 по 4 свап, 1 по 2 свап, 2 по 1 ? (разные значения, не сравнить, я выбрал так копировать)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 1 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 11]; // 1 per two
				out_buffer[i + 11] = buffer[i + 10];
				out_buffer[i + 12] = buffer[i + 12]; // skip 2
				out_buffer[i + 13] = buffer[i + 13];
				i += 14;
				continue;
			} else if (current_val == 0x584C434E) { // XLCN
			// При CELL, REFR, WRLD, ACHR только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5854454C) { // XTEL
			// Только при REFR и только 0x20 - 7 по 4 свап, 1 так копировать, 3 по 1 ? (там нули были, не сравнить, пусть так копируется)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 4] = buffer[i - 4]; // rewrite 4 skip
				out_buffer[i - 3] = buffer[i - 3];
				out_buffer[i - 2] = buffer[i - 2];
				out_buffer[i - 1] = buffer[i - 1];
				continue;
			} else if (current_val == 0x5853434C) { // XSCL
			// Только при REFR и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E455854) { // NEXT
			// При QUST, SCEN только 0. Зачем такой нужен?.. И ведь назвали NEXT...
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44454D4F) { // DEMO
			// Только при SCEN, и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x464C5452) { // FLTR
			// Только при QUST, string
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x444F4454) { // DODT
			// При TXST, IPCT только 0x24 - 7 по 4 свап, 4 по 1 так копировать, 1 по 4 свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 8] = buffer[i - 8]; // rewrite 4 skip
				out_buffer[i - 7] = buffer[i - 7];
				out_buffer[i - 6] = buffer[i - 6];
				out_buffer[i - 5] = buffer[i - 5];
				continue;
			} else if (current_val == 0x56454E43) { // VENC
			// Только при FACT и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x56454E44) { // VEND
			// Только при FACT и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504C5644) { // PLVD
			// Только при FACT и только 0xC - 1 по 4 так копировать, 1 по 4 свап, 1 по 4 ? (там нули были, не сравнить, radius, int наверное, пусть свапится)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 12] = buffer[i - 12]; // rewrite 4 skip
				out_buffer[i - 11] = buffer[i - 11];
				out_buffer[i - 10] = buffer[i - 10];
				out_buffer[i - 9] = buffer[i - 9];
				continue;
			} else if (current_val == 0x57414954) { // WAIT
			// Только при FACT и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53544F4C) { // STOL
			// Только при FACT и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x504C434E) { // PLCN
			// Только при FACT и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4A4F5554) { // JOUT
			// Только при FACT и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58524453) { // XRDS
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43524752) { // CRGR
			// Только при FACT и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x534E4444) { // SNDD
			// При MGEF только кратые 8 значения, свап
			// При ARMA только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53445343) { // SDSC
			// Только при SOUN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D444F42) { // MDOB
			// При MGEF, SPEL, SCRL, SHOU только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4B574441) { // KWDA
			// При RACE, MGEF, ACTI, ARMO, BOOK, INGR, MISC, FURN, WEAP, SLGM, QUST, LCTN кратные 4 значения, свап
			// При SCRL, AMMO, KEYM, ALCH только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4A41494C) { // JAIL
			// Только при FACT и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43495443) { // CITC
			// При FACT, PACK, SMBN, SMQN, MUST только 4, свап
			// При SMEN только 4 - 1 по 4 ? (там нули были, не сравнить, пусть свапится как все остальные)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49434F4E) { // ICON
			// При EFSH 1 или string, так копировать
			// При EYES, MISC, SPGD, REGN string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x48454144) { // HEAD
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5250524D) { // RPRM
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52505246) { // RPRF
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x574B4D56) { // WKMV
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x514F424A) { // QOBJ
			// Только при QUST и только 2, свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x51535441) { // QSTA
			// Только при QUST и только 8 - 1 по 4 свап, 1 так копировать, 3 по 1 ? (там нули были, не сравнить, пусть так копируется)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 1 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 10]; // skip 4
				out_buffer[i + 11] = buffer[i + 11];
				out_buffer[i + 12] = buffer[i + 12];
				out_buffer[i + 13] = buffer[i + 13];
				i += 14;
				continue;
			} else if (current_val == 0x414C5354) { // ALST
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C5043) { // ALPC
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58415052) { // XAPR
			// При REFR, ACHR только 4, не очень-то и похожие значения... но свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58415044) { // XAPD
			// При REFR, ACHR только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58455350) { // XESP
			// При REFR, ACHR, PGRE, PHZD только 8 - 1 по 4 свап, 1 так копировать, 3 по 1 ? (не сравнимые значения... я хз чё с ними делать... пусть так копируются)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 9]; // 1 per four
				out_buffer[i + 7] = buffer[i + 8];
				out_buffer[i + 8] = buffer[i + 7];
				out_buffer[i + 9] = buffer[i + 6];
				out_buffer[i + 10] = buffer[i + 10]; // skip 4
				out_buffer[i + 11] = buffer[i + 11];
				out_buffer[i + 12] = buffer[i + 12];
				out_buffer[i + 13] = buffer[i + 13];
				i += 14;
				continue;
			} else if (current_val == 0x584F574E) { // XOWN
			// При REFR, ACHR только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C4947) { // XLIG
			// Только при REFR и только 0x10, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58454D49) { // XEMI
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C4B52) { // XLKR
			// При REFR, ACHR только 8, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C5254) { // XLRT
			// При REFR, ACHR только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49434F32) { // ICO2
			// Только при EFSH, 1 или string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53504354) { // SPCT
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x46545346) { // FTSF
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5048544E) { // PHTN
			// Только при RACE, значения в диапазоне от 2 до 10, string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x524E4D56) { // RNMV
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D545950) { // MTYP
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4654534D) { // FTSM
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45545950) { // ETYP
			// При SPEL, SCRL, ARMO, WEAP только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45464954) { // EFIT
			// При ENCH, SPEL, SCRL, INGR, ALCH только 0xC, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45464944) { // EFID
			// При ENCH, SPEL, SCRL, INGR, ALCH только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44535446) { // DSTF
			// При ACTI, MSTT, FURN, PROJ только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x42414D54) { // BAMT
			// При ARMO, WEAP только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52444F54) { // RDOT
			// Только при REGN и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52504C49) { // RPLI
			// Только при REGN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52434C52) { // RCLR
			// Только при REGN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4544) { // ALED
			// Только при QUST и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C434C) { // ALCL
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4E41) { // ALNA
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4645) { // ALFE
			// Только при QUST и только 4, выглядит как string, но нужен свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C434F) { // ALCO
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4643) { // ALFC
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C5541) { // ALUA
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58504F44) { // XPOD
			// Только при REFR и только 8, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C4942) { // XLIB
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C434D) { // XLCM
			// При REFR, ACHR только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58505752) { // XPWR
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58434C57) { // XCLW
			// Только при CELL и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58434C43) { // XCLC
			// Только при CELL и только 0xC - 2 по 4 свап, 4 по 1 ? (не сравнимые значения... не похоже что свап... я хз чё с ними делать... пусть так копируются)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 4] = buffer[i - 4]; // rewrite 4 skip
				out_buffer[i - 3] = buffer[i - 3];
				out_buffer[i - 2] = buffer[i - 2];
				out_buffer[i - 1] = buffer[i - 1];
				continue;
			} else if (current_val == 0x4C544D50) { // LTMP
			// При WRLD только 4, свап
			// При CELL только 4 - 1 по 4 ? (там нули были, не сравнить, пусть свапится как в другом)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44465446) { // DFTF
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53574D56) { // SWMV
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53504544) { // SPED
			// При RACE, MOVT только 0x2C, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4446544D) { // DFTM
			// Только при RACE и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x444D4454) { // DMDT
			// Только при ACTI кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D4F4453) { // MODS
			// При ACTI, CONT, DOOR, MISC, STAT, MSTT, FLOR, FURN, WEAP, ALCH string содержащие значения - 1 по 4 свап, остальные до конца так копировать
				twoBytes_nextFour_restskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x444D444C) { // DMDL
			// Только при ACTI, string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44535444) { // DSTD
				// При ACTI, MSTT, FURN, PROJ только 0x14 - 1 по 4 стак копировать, 2 по 4 свап, 2 по 4 ? (там нули были, не сравнить, Debris и Debris Count, полагаю, свап)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 20] = buffer[i - 20]; // rewrite 4 skip
				out_buffer[i - 19] = buffer[i - 19];
				out_buffer[i - 18] = buffer[i - 18];
				out_buffer[i - 17] = buffer[i - 17];
				continue;
			} else if (current_val == 0x45414D54) { // EAMT
			// Только при WEAP и только 2, свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x42494453) { // BIDS
			// При ARMO, WEAP только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43524454) { // CRDT
			// Только при WEAP и только 0x10 - 1 по 2 свап, 1 по 2 ? (не сравнимые числа... свап наверное), 1 по 4 свап, 2 по 1 так копировать, 6 по 1 ? (не сравнимые числа и нули... я хз чё с ними делать... пусть так копируются)
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				out_buffer[i + 6] = buffer[i + 7]; // 2 per two
				out_buffer[i + 7] = buffer[i + 6];
				out_buffer[i + 8] = buffer[i + 9];
				out_buffer[i + 9] = buffer[i + 8];
				out_buffer[i + 10] = buffer[i + 13]; // 1 per four
				out_buffer[i + 11] = buffer[i + 12];
				out_buffer[i + 12] = buffer[i + 11];
				out_buffer[i + 13] = buffer[i + 10];
				out_buffer[i + 14] = buffer[i + 14]; // skip 8
				out_buffer[i + 15] = buffer[i + 15];
				out_buffer[i + 16] = buffer[i + 16];
				out_buffer[i + 17] = buffer[i + 17];
				out_buffer[i + 18] = buffer[i + 18];
				out_buffer[i + 19] = buffer[i + 19];
				out_buffer[i + 20] = buffer[i + 20];
				out_buffer[i + 21] = buffer[i + 21];
				i += 22;
				continue;
			} else if (current_val == 0x4E564D49) { // NVMI

					NVMI_Special(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x58524D52) { // XRMR
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C4F43) { // XLOC !! Guessed too much !!
				// Только при REFR и только 0x14 - 4 по 1 ? (не сравнимые значения, не похоже что свап, пусть так копируются), 1 по 4 свап, 4 по 1 ? (не сравнимые значения, не похоже что свап, пусть так копируются), 8 по 1 ? (там нули были, не сравнить, пусть так копируются как хрен знат что)
				twoBytes_nskip(buffer, out_buffer, &i);
				out_buffer[i - 16] = buffer[i - 13]; // rewrite 4 swap
				out_buffer[i - 15] = buffer[i - 14];
				out_buffer[i - 14] = buffer[i - 15];
				out_buffer[i - 13] = buffer[i - 16];
				continue;
			} else if (current_val == 0x5857434E) { // XWCN
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584D524B) { // XMRK
			// При FURN string, так копировать
			// При REFR только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54434C54) { // TCLT
			// Только при INFO и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x54574154) { // TWAT
			// Только при INFO и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4C53) { // ALLS
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4E54) { // ALNT
			// Только при QUST и только 4 - 1 по 4 ? (там нули были, не сравнить, в ПК'шном где есть, написано "Linked Ref Child", раз Ref, значит, думаю, свап)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4644) { // ALFD
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4652) { // ALFR
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49444C41) { // IDLA
			// При IDLM, PACK кратные 4 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50544441) { // PTDA
				// Только при PACK и только 0xC - 1 так копировать, 3 по 1 ? (там нули были, не сравнить, пусть так копируется) 1 по 4 свап, 1 по 4 ? (там нули были, не сравнить, "Count / Distance", я считаю, свап)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 12] = buffer[i - 12]; // rewrite 4 skip
				out_buffer[i - 11] = buffer[i - 11];
				out_buffer[i - 10] = buffer[i - 10];
				out_buffer[i - 9] = buffer[i - 9];
				continue;
			} else if (current_val == 0x50524342) { // PRCB
			// Только при PACK и только 8, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43534352) { // CSCR
			// Только при CSTY и только 0x10, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4353464C) { // CSFL
			// Только при CSTY и только 0x20, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43534744) { // CSGD
			// Только при CSTY и только 0x28, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50524B43) { // PRKC
			// только при PERK и только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58414C50) { // XALP
			// Только при REFR и только 2, свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58434E54) { // XCNT
			// При REFR только 4, свап
			// При FSTS только 0x14, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58545249) { // XTRI
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58434C52) { // XCLR
			// Только при CELL и только 4 - 1 по 4 ? (разные не сравнимые значения, но выглядит как-будто свап, я считаю, свап)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49444C46) { // IDLF
			// При IDLM, PACK только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49444C43) { // IDLC
			// При IDLM, PACK только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49444C54) { // IDLT
			// При IDLM, PACK только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x454E4954) { // ENIT
			// При INGR только 8 - 1 по 4 свап, 1 по 4 ? (там нули были, не сравнить, пусть свапится как другие)
			// При ALCH только 0x14 - 2 по 4 свап, 2 по 4 ? (там нули были, не сравнить, пусть свапится как другие), 1 по 4 свап
			// При ENCH только 0x24, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53504954) { // SPIT
			// При SPEL только 0x24, свап
			// При SCRL только 0x24 - 1 по 4 свап, 2 по 4 ? (там нули были, не сравнить, пусть свапятся как SPEL), 3 по 4 свап, 2 по 4 ? (там нули были, не сравнить, пусть свапятся как SPEL)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44455354) { // DEST
			// При MSTT, FURN только 8 - 1 по 4 свап, 4 по 1 так копировать
			// При ACTI, PROJ только 8 - 1 по 4 свап, 2 по 1 так копировать, 2 по 1 ? (не сравнимые значения или нули, "Unknown", пусть так копируются как в других)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 4] = buffer[i - 4]; // rewrite 4 skip
				out_buffer[i - 3] = buffer[i - 3];
				out_buffer[i - 2] = buffer[i - 2];
				out_buffer[i - 1] = buffer[i - 1];
				continue;
			} else if (current_val == 0x444D4453) { // DMDS
			// Только при ACTI и только 0x23, string содержащие значения - 1 по 4 свап, остальные до конца так копировать
				twoBytes_nextFour_restskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5155414C) { // QUAL
			// Только при APPA и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50465043) { // PFPC !! Может быть не точно !!
			// При TREE, FLOR только 4 - 4 по 1 ? (палиндромы, не сравнить, 4 байта "Ingredient Production" в сезоны года, полагаю, для каждого сезона свой байт слева направо, наверное, так копировать)
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50464947) { // PFIG
			// При TREE, FLOR только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x464E5052) { // FNPR
			// Только при FURN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x57424454) { // WBDT
			// Только при FURN и только 2, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C564C46) { // LVLF
			// При LVLN, LVLI, LVSP только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C564C44) { // LVLD
			// При LVLN, LVLI, LVSP только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4341) { // ALCA
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x51534454) { // QSDT
			// Только при QUST и только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C464C) { // ALFL
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4641) { // ALFA
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C444E) { // ALDN
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5843494D) { // XCIM
			// Только при CELL и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584D4252) { // XMBR
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58455A4E) { // XEZN
			// При ACHR, REFR, WRLD только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C564C47) { // LVLG
			// Только при LVLI и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C4C4354) { // LLCT
			// При LVLN, LVLI, LVSP только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x494D5350) { // IMSP
			// Только при WTHR и только 0x10, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x44414C43) { // DALC
			// При WTHR, LGTM только 0x20 - 6 по 4 свап, 1 по 4 ? (там нули были, не сравнить, пусть свапится как соседи), 1 по 4 свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52504C44) { // RPLD
			// Только при REGN кратные 8 не меньшие 32 значения, "Region Point List Data", не особо и совпадающие... я считаю, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52445754) { // RDWT
			// Только при REGN кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52445341) { // RDSA
			// Только при REGN кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x52444D4F) { // RDMO
			// Только при REGN и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58574355) { // XWCU !! Много пустот !!
			// Только при REFR и только 0x30 - 2 по 4 свап, 10 по 4 ? (там нули были, не сравнить, пусть свапятся как сосед слева)
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E414D41) { // NAMA
			// Только при WRLD и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58544E4D) { // XTNM
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45434F52) { // ECOR
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C5254) { // ALRT
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4551) { // ALEQ
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x5154474C) { // QTGL
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4649) { // ALFI
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43534D45) { // CSME
			// Только при CSTY и только 0x20, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x43534C52) { // CSLR
			// Только при CSTY и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45504654) { // EPFT
			// Только при PERK и только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x50524B46) { // PRKF
			// Только при PERK и только 0, пропускать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x42504E49) { // BPNI
			// Только при BPTD 2 или string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4250544E) { // BPTN
			// Только при BPTD и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x42504E4E) { // BPNN
			// Только при BPTD string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x42504E54) { // BPNT
			// Только при BPTD string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C434543) { // LCEC
			// Только при LCTN кратные 4 не меньшие 8 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C434944) { // LCID
			// Только при LCTN кратные 4 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C434550) { // LCEP
			// Только кратные 12 значения, которые каждые 12 байт делать так - 2 по 4 свап, 1 по 4 так копировать
				out_buffer[i + 4] = buffer[i + 5]; // two
				out_buffer[i + 5] = buffer[i + 4];
				for (long lcep = 0; lcep < *(uint16_t*)&out_buffer[i + 4]; lcep += 12) {
					out_buffer[i + 6 + lcep] = buffer[i + 9 + lcep]; // 2 per four
					out_buffer[i + 7 + lcep] = buffer[i + 8 + lcep];
					out_buffer[i + 8 + lcep] = buffer[i + 7 + lcep];
					out_buffer[i + 9 + lcep] = buffer[i + 6 + lcep];
					out_buffer[i + 10 + lcep] = buffer[i + 13 + lcep];
					out_buffer[i + 11 + lcep] = buffer[i + 12 + lcep];
					out_buffer[i + 12 + lcep] = buffer[i + 11 + lcep];
					out_buffer[i + 13 + lcep] = buffer[i + 10 + lcep];
					out_buffer[i + 14 + lcep] = buffer[i + 14 + lcep]; // skip 4
					out_buffer[i + 15 + lcep] = buffer[i + 15 + lcep];
					out_buffer[i + 16 + lcep] = buffer[i + 16 + lcep];
					out_buffer[i + 17 + lcep] = buffer[i + 17 + lcep];
				}
				i = i + 6 + *(uint16_t*)&out_buffer[i + 4];
				continue;
			} else if (current_val == 0x42504E44) { // BPND !! Много пустот !!
			// Только при BPTD и только 0x54 - 1 по 4 свап, 6 по 1 так копировать, 10 по 1 ? (там нули были, не сравнить... и Tes5Edit не особо подсказывает... я хз чё с ними делать, пусть так копируются, но это наобум), 2 по 4 свап, 3 по 4 ? (там нули были, не сравнить... "Severable - Debris_Count,Debris,Explosion", если Debris и Explosion Refs, то тут с большей долей вероятности свап (хотя дальше идут "Severable/Explodable - Impact DataSet", у них Refs какие-то каким не нужен свап...), но насчёт Debris Count может быть и не свап... но пусть свапятся), 1 по 4 свап, 6 по 4 ? (там нули были, не сравнить, "Gore Effects Positioning", floats координаты, я считаю, свап), 3 по 4 так копировать (из них последняя пара байт угадано, ато там нули, не сравнить...), 1 по 4 свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				out_buffer[i - 80] = buffer[i - 80]; // rewrite skip 16
				out_buffer[i - 79] = buffer[i - 79];
				out_buffer[i - 78] = buffer[i - 78];
				out_buffer[i - 77] = buffer[i - 77];
				out_buffer[i - 76] = buffer[i - 76];
				out_buffer[i - 75] = buffer[i - 75];
				out_buffer[i - 74] = buffer[i - 74]; // (¿questionable? further skip 10)
				out_buffer[i - 73] = buffer[i - 73];
				out_buffer[i - 72] = buffer[i - 72];
				out_buffer[i - 71] = buffer[i - 71];
				out_buffer[i - 70] = buffer[i - 70];
				out_buffer[i - 69] = buffer[i - 69];
				out_buffer[i - 68] = buffer[i - 68];
				out_buffer[i - 67] = buffer[i - 67];
				out_buffer[i - 66] = buffer[i - 66];
				out_buffer[i - 65] = buffer[i - 65];
				
				out_buffer[i - 16] = buffer[i - 16]; // rewrite skip 12
				out_buffer[i - 15] = buffer[i - 15];
				out_buffer[i - 14] = buffer[i - 14];
				out_buffer[i - 13] = buffer[i - 13];
				out_buffer[i - 12] = buffer[i - 12];
				out_buffer[i - 11] = buffer[i - 11];
				out_buffer[i - 10] = buffer[i - 10];
				out_buffer[i - 9] = buffer[i - 9];
				out_buffer[i - 8] = buffer[i - 8];
				out_buffer[i - 7] = buffer[i - 7];
				out_buffer[i - 6] = buffer[i - 6]; // (¿questionable? further skip 2)
				out_buffer[i - 5] = buffer[i - 5];
				continue;
			} else if (current_val == 0x534F554C) { // SOUL
			// Только при SLGM и только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58414354) { // XACT
			// Только при REFR и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C4541) { // ALEA
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x414C5350) { // ALSP
			// Только при QUST и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45504644) { // EPFD !! могут быть как свап, так и string !!
			// Значения при PERK:
			//4, 8 - свап,
			// Остальные string так копировать. Не вижу никакого другого выхода, кроме как свапать когда 4 или 8. Соответственно, если есть где-то 4 или 8 string, то ему не повезло...
				if ((buffer[i + 5] == 4) || (buffer[i + 5] == 8))
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				else
					twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4156534B) { // AVSK
			// Только при AVIF и только 0x10, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C435052) { // LCPR
			// Только при LCTN кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x49545854) { // ITXT
			// Только при MESG и только 4, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D505254) { // MPRT
			// Только при ASTP string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x46505254) { // FPRT
			// Только при ASTP string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x46434854) { // FCHT
			// Только при ASTP string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D434854) { // MCHT
			// Только при ASTP string, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
// CLDC is only one in Skyrim.esm. Length 0x10. It needs a straight byteswapping.
// Swapped by GRUP algorhitm.
			} else if (current_val == 0x534C4350) { // SLCP
			// Только при SLGM и только 1, так копировать
				twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x57435452) { // WCTR
			// Только при WRLD и только 4 - 2 по 2 свап
				twoBytes_nextPerTwo(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4F465354) { // OFST
				// Только при WRLD (нуу и один затесался вовнутрь XXXX...), огромные массивы с не особо сравнимыми значениями, которые, по всей видимости, нужно байтсваппить по 4.
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4C43554E) { // LCUN
			// Только при LCTN кратные 12 значения, свап
				twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45504632) { // EPF2
				// Только при PERK и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x45504633) { // EPF3
				// Только при PERK и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4D484454) { // MHDT !! предположено !!
				// Довольно мало в Skyrim.esm. Вывод сделан по тем, что есть...
					twoBytes_nextFourPerTwo_restskip(buffer, out_buffer, &i);
				continue;
			} else if (((current_val & 0x00FFFFFF) == 0x494144) && (((current_val >> 24) < 0x15) || (((current_val >> 24) > 0x3F) && ((current_val >> 24) < 0x55)))) { // Все ƔIAD
				// Абсолютно все кратные 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E41564D) { // NAVM !!! check!

				global_nskip(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4E50435F) { // NPC

				global_nskip(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4F544654) { // OTFT !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x5041434B) { // PACK !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x5045524B) { // PERK !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x50475245) { // PGRE !!! check!

				straightFourTeen_Complicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x50485A44) { // PHZD !!! check!

				straightFourTeen_Complicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x50524F4A) { // PROJ !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
// PWAT is only one in Skyrim.esm. Length 0x10. It might be it needs
// firstOxlO_nextComplicated
// Swapped by GRUP algorhitm.
			} else if (current_val == 0x51555354) { // QUST

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x52414345) { // RACE

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x52454652) { // REFR

				straightFourTeen_Complicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x5245474E) { // REGN

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x52454C41) { // RELA

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x52464354) { // RFCT

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
// RGDL is only one in Skyrim.esm. Length 0x10. It might be it needs
// firstOxlO_nextComplicated
// Swapped by GRUP algorhitm.
			} else if (current_val == 0x5343454E) { // SCEN

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
// SCOL and SCPT are the only one in Skyrim.esm. Length 0x10. It might be they need
// firstOxlO_nextComplicated
// Swapped by GRUP algorhitm.
			} else if (current_val == 0x5343524C) { // SCRL

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x53484F55) { // SHOU

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534C474D) { // SLGM

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534D424E) { // SMBN

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534D454E) { // SMEN

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534D514E) { // SMQN

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534E4354) { // SNCT

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534E4452) { // SNDR

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534F504D) { // SOPM

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x534F554E) { // SOUN

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x5350454C) { // SPEL

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x53504744) { // SPGD

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x53544154) { // STAT

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x54414354) { // TACT

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x54524545) { // TREE

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x54585354) { // TXST

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x56545950) { // VTYP

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x57415452) { // WATR

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x57454150) { // WEAP

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x574F4F50) { // WOOP

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x57524C44) { // WRLD

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x57544852) { // WTHR

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x41535450) { // ASTP !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x52455642) { // REVB

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x45594553) { // EYES !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x45515550) { // EQUP !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4455414C) { // DUAL !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x444F424A) { // DOBJ !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x52444D50) { // RDMP
				// Семь штук при REGN и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x56484754) { // VHGT !!! Эксклюзивно не на ПК
				// Только 0x0448 8 в PS3's Skyrim.esm. Их длины чётные, но что с ними делать хз, сравнить не с чем. На вид какие-то сырые байты... Пускай так и копируются все как есть без изменений. Всего три штуки.
					twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x41545854) { // ATXT !!! Эксклюзивно не на ПК
				// Только 8 в PS3's Skyrim.esm. Я считаю, свап. Их всего пара штук длиной 8.
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x56545854) { // VTXT !!! Эксклюзивно не на ПК
				// Кратные 4 значения в PS3's Skyrim.esm. Я считаю, свап. Их всего пара штук длинами 0x18 и 0x48.
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58585858) { // XXXX

					XXXX_Special(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x5843564C) { // XCVL !! предположено !!
				// Семь штук при REFR и только 0xC - 1 по 4 ?, 1 по 4 свап, 1 по 4 ? (там нули были где "?", не сравнить, пусть свапится)
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x574C5354) { // WLST
				// Шесть штук при CLMT и только 0xC, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x534E4D56) { // SNMV
				// Пять штук при RACE и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58524E4B) { // XRNK
				// Четыре штуки при REFR и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x464E4D4B) { // FNMK
				// Четыре штуки при FURN и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x564E4D4C) { // VNML !!! Эксклюзивно не на ПК
				// Только 0x0CC3 в PS3's Skyrim.esm. Я считаю, копировать как есть, т.к. их длины нечётные. Всего три штуки.
					twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58535043) { // XSPC
				// Три штуки при REFR и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58434153) { // XCAS
				// Три штуки при CELL и только 4, свап. На ПК'шной версии их больше...
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58434D4F) { // XCMO
				// Три штуки при CELL и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x464C4D56) { // FLMV
				// Три штуки при RACE и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58484F52) { // XHOR
				// Три штуки при ACHR и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58465643) { // XFVC
				// Две штуки при REFR и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x434F4544) { // COED
				// Две штуки при CONT, LVLN и только 0xC - 1 по 4 свап, 1 по 4 ? (палиндром, не сравнить, "Required Rank", я считаю, свап), 1 по 4 свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58435A41) { // XCZA
				// Одна штука длиной в 4 в Skyrim.esm, и то со значением 0. Нафига?..
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E565050) { // NVPP
				// Одна штука длиной в 0x6460 в Skyrim.esm, и весь свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58435A43) { // XCZC
				// Одна штука длиной в 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x58485457) { // XHTW
				// Одна штука длиной в 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x584C5457) { // XLTW
				// Одна штука длиной в 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x53504F52) { // SPOR
				// Одна штука при QUST и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E564552) { // NVER
				// Одна штука при NAVI и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x4E415649) { // NAVI !!! check!

				firstOxlO_nextComplicated(buffer, out_buffer, &i);

				continue;
			} else if (current_val == 0x4D415354) { // MAST
				// Количество присутствующих соответствует количеству мастер-файлов, на которых
				// базируется плагин. String, так копировать
					twoBytes_nskip(buffer, out_buffer, &i);
				continue;
			} else if (current_val == 0x48454452) { // HEDR
				// Одна штука при вынесенном за цикл TES4 и только 4, свап
					twoBytes_nextPerFour(buffer, out_buffer, &i);
				continue;
			} else {
/*				int found_smth = 0;
				for (long k = 0; k < (long)(sizeof(signature_types) >> 2); k++) {
					if (current_val == signature_types[k]) {
						found_smth = 1;
						break;
					}
				}
				if ((!found_smth) &&
                    ((current_val >> 24) > 0x40) && ((current_val >> 24) < 0x5B) &&
                    ((current_val & 0x00FF0000) > 0x00400000) && ((current_val & 0x00FF0000) < 0x005B0000) &&
                    ((current_val & 0x0000FF00) > 0x00004000) && ((current_val & 0x0000FF00) < 0x00005B00) &&
                    ((current_val & 0x000000FF) > 0x40) && ((current_val & 0x000000FF) < 0x5B) &&
					(buffer[i + 4] == 0))
                {
                    printf("New type 0x%08X\n", current_val);
                }*/
				printf("unknown offset 0x%08X\n", i);
				i += 4;
			}
		} else {
			while (i < filesize) {
			out_buffer[i] = buffer[i];
				i++;
			}
		}
	}

    FILE *outfile = fopen("Skyrim_.esm", "wb");
    if (!outfile) { perror("Error output"); free(buffer); free(out_buffer); return 1; }
    
    fwrite(out_buffer, 1, filesize, outfile);
    fclose(outfile);

    free(buffer);
    free(out_buffer);

    printf("Done\n");
    return 0;
}
