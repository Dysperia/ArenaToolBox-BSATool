#include "image.hpp"
#include "various.hpp"
#include <QImage>
#include <iostream>
#include <fstream>

// Constructor
Image::Image() : hasTransparency(false)
{
    // Initializing image data to one frame
    animationData.resize(1);

    //Initialysing palette string
    palette = "PAL.COL";

    // Stream to read palette file
    std::ifstream paletteStream;

    // building map of native IMG/COL filename and their respective palette
    // filename (if COL file or IMG with integrated palette)
    {
        nativePaletteMap["AUTOMAP.IMG"] = "AUTOMAP.COL";
        nativePaletteMap["BLAKMRSH.IMG"] = "BLAKMRSH.COL";
        nativePaletteMap["BUYSPELL.IMG"] = "BUYSPELL.COL";
        nativePaletteMap["CHARSHT.COL"] = "CHARSHT.COL";
        nativePaletteMap["CHARSPEL.IMG"] = "CHARSPEL.COL";
        nativePaletteMap["COLOSSUS.IMG"] = "COLOSSUS.COL";
        nativePaletteMap["CRYPT.IMG"] = "CRYPT.COL";
        nativePaletteMap["DAGOTHUR.IMG"] = "DAGOTHUR.COL";
        nativePaletteMap["DAYTIME.COL"] = "DAYTIME.COL";
        nativePaletteMap["DREARY.COL"] = "DREARY.COL";
        nativePaletteMap["ELSWEYR.IMG"] = "ELSWEYR.COL";
        nativePaletteMap["FANGLAIR.IMG"] = "FANGLAIR.COL";
        nativePaletteMap["GROVE.IMG"] = "GROVE.COL";
        nativePaletteMap["HAMERFEL.IMG"] = "HAMERFEL.COL";
        nativePaletteMap["HIGHROCK.IMG"] = "HIGHROCK.COL";
        nativePaletteMap["HISTORY.IMG"] = "HISTORY.COL";
        nativePaletteMap["HOUSE.IMG"] = "HOUSE.COL";
        nativePaletteMap["IMPERIAL.IMG"] = "IMPERIAL.COL";
        nativePaletteMap["LABRINTH.IMG"] = "LABRINTH.COL";
        nativePaletteMap["LOGBOOK.IMG"] = "LOGBOOK.COL";
        nativePaletteMap["MAPBTNS.MNU"] = "MAPBTNS.COL";
        nativePaletteMap["MENU.IMG"] = "MENU.COL";
        nativePaletteMap["MENU_CTR.IMG"] = "MENU_CTR.COL";
        nativePaletteMap["MENU_LFT.IMG"] = "MENU_LFT.COL";
        nativePaletteMap["MENU_RT.IMG"] = "MENU_RT.COL";
        nativePaletteMap["MENUSCRN.IMG"] = "MENUSCRN.COL";
        nativePaletteMap["MIRKWOOD.IMG"] = "MIRKWOOD.COL";
        nativePaletteMap["MOROWIND.IMG"] = "MOROWIND.COL";
        nativePaletteMap["PAL.COL"] = "PAL.COL";
        nativePaletteMap["PARCH.IMG"] = "PARCH.COL";
        nativePaletteMap["QUOTE.IMG"] = "QUOTE.COL";
        nativePaletteMap["SCROLL03.IMG"] = "SCROLL03.COL";
        nativePaletteMap["SKULL.IMG"] = "SKULL.COL";
        nativePaletteMap["SKYRIM.IMG"] = "SKYRIM.COL";
        nativePaletteMap["SPELLMKR.IMG"] = "SPELLMKR.COL";
        nativePaletteMap["STARTGAM.MNU"] = "STARTGAM.COL";
        nativePaletteMap["SUMERSET.IMG"] = "SUMERSET.COL";
        nativePaletteMap["TAMRIEL.IMG"] = "TAMRIELIMG.COL";
        nativePaletteMap["TAMRIEL.MNU"] = "TAMRIELMNU.COL";
        nativePaletteMap["TEST.IMG"] = "TEST.COL";
        nativePaletteMap["TITLE.IMG"] = "TITLE.COL";
        nativePaletteMap["TOWER.IMG"] = "TOWER.COL";
        nativePaletteMap["VALNWOOD.IMG"] = "VALNWOOD.COL";
    }

    for(std::map<std::string, std::string>::iterator it=nativePaletteMap.begin(); it!=nativePaletteMap.end(); it++)
    {
        nativePaletteFileList.append(QString::fromStdString(it->first));
    }
    int nativeFileListNbr = nativePaletteFileList.size();
    for (int i(0); i<nativeFileListNbr; i++)
    {
        std::string paletteFileName = nativePaletteFileList[i].toStdString();
        std::string paletteName = nativePaletteMap[paletteFileName];
        std::string path = "nativePalette/" + paletteName;
        paletteStream.open(path, std::ios_base::in | std::ios_base::binary);
        if (paletteStream.is_open())
        {
            uint8_t R(0), G(0), B(0);
            paletteStream.seekg(8, std::ios_base::beg);
            // set first color to transparent or not according to palette map
            paletteStream.read(reinterpret_cast<char*>(&R), 1);
            paletteStream.read(reinterpret_cast<char*>(&G), 1);
            paletteStream.read(reinterpret_cast<char*>(&B), 1);
            transparentPalettesMap[paletteFileName].append(qRgba(R, G, B, 1));
            palettesMap[paletteFileName].append(qRgb(R, G, B));
            // set up other color
            for (size_t j(0); j<256; j++)
            {
                paletteStream.read(reinterpret_cast<char*>(&R), 1);
                paletteStream.read(reinterpret_cast<char*>(&G), 1);
                paletteStream.read(reinterpret_cast<char*>(&B), 1);
                transparentPalettesMap[paletteFileName].append(qRgb(R, G, B));
                palettesMap[paletteFileName].append(qRgb(R, G, B));
            }
            paletteStream.close();
        }
    }

    palettesMap["Integrated"] = QVector<QRgb>();
    transparentPalettesMap["Integrated"] = QVector<QRgb>();

    // Setting up native Arena IMG list
    {
        nativeIMGsList[0] = {"0ARGHELM.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[1] = {"1ARGHELM.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[2] = {"ACCPREJT.IMG", "PAL.COL", "8"};
        nativeIMGsList[3] = {"ARENARW.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[4] = {"ARROW01.IMG", "PAL.COL", "0"};
        nativeIMGsList[5] = {"AUTOMAP.IMG", "Integrated", "1"};
        nativeIMGsList[6] = {"BAGAGE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[7] = {"BARREL.IMG", "PAL.COL", "0"};
        nativeIMGsList[8] = {"BEAKERS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[9] = {"BEAKERS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[10] = {"BEAKERS3.IMG", "PAL.COL", "0"};
        nativeIMGsList[11] = {"BED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[12] = {"BK01COL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[13] = {"BK01COLD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[14] = {"BK01PAL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[15] = {"BK01PALD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[16] = {"BK02PAL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[17] = {"BK02PALD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[18] = {"BKFACESF.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[19] = {"BLAKMRSH.IMG", "Integrated", "1"};
        nativeIMGsList[20] = {"BLUERUG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[21] = {"BOATPROL.IMG", "PAL.COL", "8"};
        nativeIMGsList[22] = {"BOATQURT.IMG", "PAL.COL", "0"};
        nativeIMGsList[23] = {"BOATSIDE.IMG", "PAL.COL", "0"};
        nativeIMGsList[24] = {"BODY06.IMG", "PAL.COL", "0"};
        nativeIMGsList[25] = {"BONES1.IMG", "PAL.COL", "0"};
        nativeIMGsList[26] = {"BONES3.IMG", "PAL.COL", "0"};
        nativeIMGsList[27] = {"BONES5.IMG", "PAL.COL", "0"};
        nativeIMGsList[28] = {"BONUS.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[29] = {"BOOKS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[30] = {"BOOKS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[31] = {"BRACE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[32] = {"BS_A1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[33] = {"BS_A2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[34] = {"BS_A3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[35] = {"BS_ADOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[36] = {"BS_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[37] = {"BS_B2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[38] = {"BS_B3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[39] = {"BS_BDOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[40] = {"BS_C1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[41] = {"BS_C2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[42] = {"BS_CDOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[43] = {"BUYSPELL.IMG", "Integrated", "1"};
        nativeIMGsList[44] = {"CACTUS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[45] = {"CACTUS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[46] = {"CACTUS3.IMG", "PAL.COL", "0"};
        nativeIMGsList[47] = {"CASD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[48] = {"CASLAVA.IMG", "PAL.COL", "texture"};
        nativeIMGsList[49] = {"CASPIT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[50] = {"CASU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[51] = {"CCELL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[52] = {"CELDOOR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[53] = {"CELL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[54] = {"CELLDOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[55] = {"CHAINS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[56] = {"CHAINS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[57] = {"CHAIR3.IMG", "PAL.COL", "0"};
        nativeIMGsList[58] = {"CHARBK00.IMG", "Integrated", "9"};
        nativeIMGsList[59] = {"CHARBK01.IMG", "Integrated", "9"};
        nativeIMGsList[60] = {"CHARBK02.IMG", "Integrated", "9"};
        nativeIMGsList[61] = {"CHARBK03.IMG", "Integrated", "9"};
        nativeIMGsList[62] = {"CHARBK04.IMG", "Integrated", "9"};
        nativeIMGsList[63] = {"CHARBK05.IMG", "Integrated", "9"};
        nativeIMGsList[64] = {"CHARBK06.IMG", "Integrated", "9"};
        nativeIMGsList[65] = {"CHARBK07.IMG", "Integrated", "9"};
        nativeIMGsList[66] = {"CHARSPEL.IMG", "Integrated", "1"};
        nativeIMGsList[67] = {"CHARSTAT.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[68] = {"CHESTC.IMG", "PAL.COL", "0"};
        nativeIMGsList[69] = {"CHESTO.IMG", "PAL.COL", "0"};
        nativeIMGsList[70] = {"CHRBKF00.IMG", "Integrated", "9"};
        nativeIMGsList[71] = {"CHRBKF01.IMG", "Integrated", "9"};
        nativeIMGsList[72] = {"CHRBKF02.IMG", "Integrated", "9"};
        nativeIMGsList[73] = {"CHRBKF03.IMG", "Integrated", "9"};
        nativeIMGsList[74] = {"CHRBKF04.IMG", "Integrated", "9"};
        nativeIMGsList[75] = {"CHRBKF05.IMG", "Integrated", "9"};
        nativeIMGsList[76] = {"CHRBKF06.IMG", "Integrated", "9"};
        nativeIMGsList[77] = {"CHRBKF07.IMG", "Integrated", "9"};
        nativeIMGsList[78] = {"CITY.IMG", "VALNWOOD.IMG", "noHeader"};
        nativeIMGsList[79] = {"CITYWALD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[80] = {"CITYWALL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[81] = {"CITYWALX.IMG", "PAL.COL", "texture"};
        nativeIMGsList[82] = {"CLOTHES.IMG", "PAL.COL", "0"};
        nativeIMGsList[83] = {"CLOUD01.IMG", "PAL.COL", "8"};
        nativeIMGsList[84] = {"CLOUD02.IMG", "PAL.COL", "8"};
        nativeIMGsList[85] = {"CLOUD03.IMG", "PAL.COL", "8"};
        nativeIMGsList[86] = {"CLOUD04.IMG", "PAL.COL", "8"};
        nativeIMGsList[87] = {"CLOUD05.IMG", "PAL.COL", "8"};
        nativeIMGsList[88] = {"CLOUD06.IMG", "PAL.COL", "8"};
        nativeIMGsList[89] = {"CLOUD07.IMG", "PAL.COL", "8"};
        nativeIMGsList[90] = {"CLOUD08.IMG", "PAL.COL", "8"};
        nativeIMGsList[91] = {"CLOUD09.IMG", "PAL.COL", "8"};
        nativeIMGsList[92] = {"CLOUD10.IMG", "PAL.COL", "8"};
        nativeIMGsList[93] = {"CLOUD11.IMG", "PAL.COL", "8"};
        nativeIMGsList[94] = {"CLOUD12.IMG", "PAL.COL", "8"};
        nativeIMGsList[95] = {"CLOUD13.IMG", "PAL.COL", "8"};
        nativeIMGsList[96] = {"CLOUD14.IMG", "PAL.COL", "0"};
        nativeIMGsList[97] = {"CLOUD15.IMG", "PAL.COL", "0"};
        nativeIMGsList[98] = {"CLOUD16.IMG", "PAL.COL", "0"};
        nativeIMGsList[99] = {"CLOUD17.IMG", "PAL.COL", "0"};
        nativeIMGsList[100] = {"COFFIN1.IMG", "PAL.COL", "0"};
        nativeIMGsList[101] = {"COFFIN2.IMG", "PAL.COL", "0"};
        nativeIMGsList[102] = {"COLOSSUS.IMG", "Integrated", "1"};
        nativeIMGsList[103] = {"COMPASS.IMG", "PAL.COL", "0"};
        nativeIMGsList[104] = {"CRYPT.IMG", "Integrated", "1"};
        nativeIMGsList[105] = {"DAGOTHUR.IMG", "Integrated", "1"};
        nativeIMGsList[106] = {"DBARN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[107] = {"DBS1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[108] = {"DBS1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[109] = {"DBS2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[110] = {"DBS2D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[111] = {"DBS3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[112] = {"DBS3D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[113] = {"DCAS1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[114] = {"DCAS2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[115] = {"DCAS3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[116] = {"DCAS4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[117] = {"DCAS5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[118] = {"DCAS6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[119] = {"DCRYPT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[120] = {"DEADBODY.IMG", "PAL.COL", "0"};
        nativeIMGsList[121] = {"DEQ.IMG", "PAL.COL", "texture"};
        nativeIMGsList[122] = {"DEQD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[123] = {"DESERT1.IMG", "PAL.COL", "8"};
        nativeIMGsList[124] = {"DESERT2.IMG", "PAL.COL", "8"};
        nativeIMGsList[125] = {"DESERT3.IMG", "PAL.COL", "8"};
        nativeIMGsList[126] = {"DESERT4.IMG", "PAL.COL", "8"};
        nativeIMGsList[127] = {"DFARM.IMG", "PAL.COL", "texture"};
        nativeIMGsList[128] = {"DGATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[129] = {"DIAMOND.IMG", "PAL.COL", "0"};
        nativeIMGsList[130] = {"DITHER.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[131] = {"DITHER2.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[132] = {"DLGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[133] = {"DLGTD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[134] = {"DMU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[135] = {"DMUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[136] = {"DNB1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[137] = {"DNB1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[138] = {"DNG2LAVA.IMG", "PAL.COL", "texture"};
        nativeIMGsList[139] = {"DNG2PIT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[140] = {"DNG2WATR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[141] = {"DOOR1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[142] = {"DOOR1D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[143] = {"DOOR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[144] = {"DOOR2D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[145] = {"DOOR3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[146] = {"DOOR3D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[147] = {"DOOR4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[148] = {"DOOR4D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[149] = {"DOOR5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[150] = {"DOOR5D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[151] = {"DOOR6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[152] = {"DOOR6D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[153] = {"DOOR_1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[154] = {"DOOR_2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[155] = {"DOOR_3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[156] = {"DOOR_4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[157] = {"DOOR_5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[158] = {"DOOR_6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[159] = {"DORI1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[160] = {"DORI2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[161] = {"DORI3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[162] = {"DRGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[163] = {"DRGTD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[164] = {"DTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[165] = {"DTAVD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[166] = {"DTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[167] = {"DTEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[168] = {"DTOWER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[169] = {"DUNGEON.IMG", "VALNWOOD.IMG", "noHeader"};
        nativeIMGsList[170] = {"DZDBARN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[171] = {"DZDCRYPT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[172] = {"DZDFARM.IMG", "PAL.COL", "texture"};
        nativeIMGsList[173] = {"DZDTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[174] = {"DZDTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[175] = {"DZDTOWER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[176] = {"DZNFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[177] = {"DZRIVER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[178] = {"DZTTAV.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[179] = {"DZTTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[180] = {"DZTTOWER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[181] = {"ELSWEYR.IMG", "Integrated", "1"};
        nativeIMGsList[182] = {"EQUIP.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[183] = {"EQUIPB.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[184] = {"EQUIPMEN.IMG", "PAL.COL", "0"};
        nativeIMGsList[185] = {"EQ_A1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[186] = {"EQ_A2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[187] = {"EQ_A3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[188] = {"EQ_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[189] = {"EQ_B2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[190] = {"EQ_B3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[191] = {"EQ_B4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[192] = {"EQ_DOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[193] = {"FANGLAIR.IMG", "Integrated", "1"};
        nativeIMGsList[194] = {"FLAG.IMG", "PAL.COL", "0"};
        nativeIMGsList[195] = {"FLCAS_A1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[196] = {"FLCAS_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[197] = {"FLCAS_E1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[198] = {"FLIZ0.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[199] = {"FLIZ1.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[200] = {"FLIZ2.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[201] = {"FLOORA1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[202] = {"FLOORA2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[203] = {"FLOORA3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[204] = {"FLOORB1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[205] = {"FLOORB2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[206] = {"FLOORB3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[207] = {"FLOORC.IMG", "PAL.COL", "texture"};
        nativeIMGsList[208] = {"FLOORC1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[209] = {"FLOORC2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[210] = {"FLOORC3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[211] = {"FLOORD01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[212] = {"FLOORD1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[213] = {"FLOORD2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[214] = {"FLOORD3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[215] = {"FLOORE1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[216] = {"FLOORF1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[217] = {"FLOOR_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[218] = {"FLOOR_C0.IMG", "PAL.COL", "texture"};
        nativeIMGsList[219] = {"FLOOR_C1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[220] = {"FLOOR_C2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[221] = {"FLOOR_C3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[222] = {"FLOOR_C4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[223] = {"FLOOR_C5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[224] = {"FLOOR_C6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[225] = {"FLOOR_C7.IMG", "PAL.COL", "texture"};
        nativeIMGsList[226] = {"FLOOR_C8.IMG", "PAL.COL", "texture"};
        nativeIMGsList[227] = {"FLOOR_C9.IMG", "PAL.COL", "texture"};
        nativeIMGsList[228] = {"FOAK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[229] = {"FOAK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[230] = {"FOAK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[231] = {"FORM1.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[232] = {"FORM10.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[233] = {"FORM11.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[234] = {"FORM12.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[235] = {"FORM13.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[236] = {"FORM14.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[237] = {"FORM15.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[238] = {"FORM2.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[239] = {"FORM3.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[240] = {"FORM4.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[241] = {"FORM4A.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[242] = {"FORM5.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[243] = {"FORM6.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[244] = {"FORM6A.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[245] = {"FORM7.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[246] = {"FORM8.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[247] = {"FORM9.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[248] = {"FOROOZ1.IMG", "CHARSPEL.IMG", "8"};
        nativeIMGsList[249] = {"FPANTS.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[250] = {"FRSHIRT.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[251] = {"FSSHIRT.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[252] = {"GARDENGD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[253] = {"GARDENGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[254] = {"GATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[255] = {"GLOBE.IMG", "PAL.COL", "0"};
        nativeIMGsList[256] = {"GOLD.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[257] = {"GROVE.IMG", "Integrated", "1"};
        nativeIMGsList[258] = {"GUARD1D.IMG", "PAL.COL", "0"};
        nativeIMGsList[259] = {"GUARD1M.IMG", "PAL.COL", "0"};
        nativeIMGsList[260] = {"GUARD2D.IMG", "PAL.COL", "0"};
        nativeIMGsList[261] = {"GUARD2M.IMG", "PAL.COL", "0"};
        nativeIMGsList[262] = {"GUARD3D.IMG", "PAL.COL", "0"};
        nativeIMGsList[263] = {"GUARD3M.IMG", "PAL.COL", "0"};
        nativeIMGsList[264] = {"HAMERFEL.IMG", "Integrated", "1"};
        nativeIMGsList[265] = {"HAMMER.IMG", "PAL.COL", "0"};
        nativeIMGsList[266] = {"HEAD.IMG", "PAL.COL", "0"};
        nativeIMGsList[267] = {"HEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[268] = {"HIGHROCK.IMG", "Integrated", "1"};
        nativeIMGsList[269] = {"HISTORY.IMG", "Integrated", "1"};
        nativeIMGsList[270] = {"HOOKAH.IMG", "PAL.COL", "0"};
        nativeIMGsList[271] = {"HOUSE.IMG", "Integrated", "1"};
        nativeIMGsList[272] = {"IDOOR1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[273] = {"IDOOR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[274] = {"IDOOR3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[275] = {"IDOOR4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[276] = {"IDOOR5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[277] = {"IDOOR6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[278] = {"IFLOOR1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[279] = {"IFLOOR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[280] = {"IFLOOR3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[281] = {"IFLOOR4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[282] = {"IFLOOR5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[283] = {"IFLOOR6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[284] = {"IMAIDEN.IMG", "PAL.COL", "0"};
        nativeIMGsList[285] = {"IMPERIAL.IMG", "Integrated", "1"};
        nativeIMGsList[286] = {"INTRO01.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[287] = {"INTRO02.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[288] = {"INTRO03.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[289] = {"INTRO04.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[290] = {"INTRO05.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[291] = {"INTRO06.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[292] = {"INTRO07.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[293] = {"INTRO08.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[294] = {"INTRO09.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[295] = {"INTRO1.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[296] = {"INTRO2.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[297] = {"INTRO3.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[298] = {"INTRO4.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[299] = {"INTRO5.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[300] = {"INTRO6.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[301] = {"INTRO7.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[302] = {"IPIT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[303] = {"ISD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[304] = {"ISDN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[305] = {"ISU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[306] = {"ISUP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[307] = {"IWATER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[308] = {"JUGS.IMG", "PAL.COL", "0"};
        nativeIMGsList[309] = {"KEY.IMG", "PAL.COL", "0"};
        nativeIMGsList[310] = {"KING.IMG", "PAL.COL", "0"};
        nativeIMGsList[311] = {"KITCHN01.IMG", "PAL.COL", "0"};
        nativeIMGsList[312] = {"KITCHN03.IMG", "PAL.COL", "0"};
        nativeIMGsList[313] = {"KITCHN06.IMG", "PAL.COL", "0"};
        nativeIMGsList[314] = {"KITCHN07.IMG", "PAL.COL", "0"};
        nativeIMGsList[315] = {"KITCHN08.IMG", "PAL.COL", "0"};
        nativeIMGsList[316] = {"KOAK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[317] = {"KOAK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[318] = {"KOAK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[319] = {"LABRINTH.IMG", "Integrated", "1"};
        nativeIMGsList[320] = {"LAUND1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[321] = {"LAUND2D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[322] = {"LAUNDRY.IMG", "PAL.COL", "texture"};
        nativeIMGsList[323] = {"LAUNDRY1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[324] = {"LAUNDRY2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[325] = {"LAUNDRYD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[326] = {"LAVA.IMG", "PAL.COL", "texture"};
        nativeIMGsList[327] = {"LAVAWALL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[328] = {"LOADSAVE.IMG", "PAL.COL", "0"};
        nativeIMGsList[329] = {"LOGBOOK.IMG", "Integrated", "1"};
        nativeIMGsList[330] = {"MAN01.IMG", "PAL.COL", "0"};
        nativeIMGsList[331] = {"MAN02.IMG", "PAL.COL", "0"};
        nativeIMGsList[332] = {"MAN03.IMG", "PAL.COL", "0"};
        nativeIMGsList[333] = {"MAN04.IMG", "PAL.COL", "0"};
        nativeIMGsList[334] = {"MAP.IMG", "PAL.COL", "0"};
        nativeIMGsList[335] = {"MCELL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[336] = {"MDOOR1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[337] = {"MDOOR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[338] = {"MDOOR3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[339] = {"MDOOR4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[340] = {"MDOOR5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[341] = {"MDOOR6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[342] = {"MENU.IMG", "Integrated", "1"};
        nativeIMGsList[343] = {"MENUSCRN.IMG", "Integrated", "1"};
        nativeIMGsList[344] = {"MENU_CTR.IMG", "Integrated", "1"};
        nativeIMGsList[345] = {"MENU_LFT.IMG", "Integrated", "1"};
        nativeIMGsList[346] = {"MENU_RT.IMG", "Integrated", "1"};
        nativeIMGsList[347] = {"MFLOORB.IMG", "PAL.COL", "texture"};
        nativeIMGsList[348] = {"MGATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[349] = {"MIRKWOOD.IMG", "Integrated", "1"};
        nativeIMGsList[350] = {"MLAVA.IMG", "PAL.COL", "texture"};
        nativeIMGsList[351] = {"MLIZ0.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[352] = {"MLIZ1.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[353] = {"MLIZ2.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[354] = {"MOROWIND.IMG", "Integrated", "1"};
        nativeIMGsList[355] = {"MOUNT001.IMG", "PAL.COL", "8"};
        nativeIMGsList[356] = {"MOUNT002.IMG", "PAL.COL", "8"};
        nativeIMGsList[357] = {"MOUNT003.IMG", "PAL.COL", "8"};
        nativeIMGsList[358] = {"MOUNT004.IMG", "PAL.COL", "8"};
        nativeIMGsList[359] = {"MOUNT005.IMG", "PAL.COL", "8"};
        nativeIMGsList[360] = {"MOUNT006.IMG", "PAL.COL", "8"};
        nativeIMGsList[361] = {"MOUNT007.IMG", "PAL.COL", "8"};
        nativeIMGsList[362] = {"MOUNT008.IMG", "PAL.COL", "8"};
        nativeIMGsList[363] = {"MOUNT009.IMG", "PAL.COL", "8"};
        nativeIMGsList[364] = {"MOUNT010.IMG", "PAL.COL", "8"};
        nativeIMGsList[365] = {"MOUNT011.IMG", "PAL.COL", "8"};
        nativeIMGsList[366] = {"MOUNT012.IMG", "PAL.COL", "8"};
        nativeIMGsList[367] = {"MOUNT013.IMG", "PAL.COL", "8"};
        nativeIMGsList[368] = {"MOUNT014.IMG", "PAL.COL", "8"};
        nativeIMGsList[369] = {"MOUNT015.IMG", "PAL.COL", "8"};
        nativeIMGsList[370] = {"MOUNT016.IMG", "PAL.COL", "8"};
        nativeIMGsList[371] = {"MOUNT017.IMG", "PAL.COL", "8"};
        nativeIMGsList[372] = {"MOUNT018.IMG", "PAL.COL", "8"};
        nativeIMGsList[373] = {"MOUNT019.IMG", "PAL.COL", "8"};
        nativeIMGsList[374] = {"MOUNT020.IMG", "PAL.COL", "8"};
        nativeIMGsList[375] = {"MOUNT021.IMG", "PAL.COL", "8"};
        nativeIMGsList[376] = {"MOUNT022.IMG", "PAL.COL", "8"};
        nativeIMGsList[377] = {"MPANTS.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[378] = {"MRSHIRT.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[379] = {"MSD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[380] = {"MSSHIRT.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[381] = {"MSU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[382] = {"MURAL1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[383] = {"MURAL2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[384] = {"MURAL3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[385] = {"MU_1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[386] = {"MU_2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[387] = {"MU_3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[388] = {"MU_4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[389] = {"MU_5.IMG", "PAL.COL", "texture"};
        nativeIMGsList[390] = {"MU_6.IMG", "PAL.COL", "texture"};
        nativeIMGsList[391] = {"MU_DOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[392] = {"MWATER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[393] = {"NALTAR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[394] = {"NBAGAGE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[395] = {"NBARREL.IMG", "PAL.COL", "0"};
        nativeIMGsList[396] = {"NBONES1.IMG", "PAL.COL", "0"};
        nativeIMGsList[397] = {"NBOULDR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[398] = {"NBOULDR2.IMG", "PAL.COL", "0"};
        nativeIMGsList[399] = {"NBOULDR3.IMG", "PAL.COL", "0"};
        nativeIMGsList[400] = {"NBOULDR4.IMG", "PAL.COL", "0"};
        nativeIMGsList[401] = {"NBOX.IMG", "PAL.COL", "0"};
        nativeIMGsList[402] = {"NBRIDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[403] = {"NBRIDGED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[404] = {"NBUSH1.IMG", "PAL.COL", "0"};
        nativeIMGsList[405] = {"NBUSH2.IMG", "PAL.COL", "0"};
        nativeIMGsList[406] = {"NBUSH3.IMG", "PAL.COL", "0"};
        nativeIMGsList[407] = {"NBUSH4.IMG", "PAL.COL", "0"};
        nativeIMGsList[408] = {"NBUSH5.IMG", "PAL.COL", "0"};
        nativeIMGsList[409] = {"NBUSH6.IMG", "PAL.COL", "0"};
        nativeIMGsList[410] = {"NBUSH7.IMG", "PAL.COL", "0"};
        nativeIMGsList[411] = {"NBUSH8.IMG", "PAL.COL", "0"};
        nativeIMGsList[412] = {"NCART1.IMG", "PAL.COL", "0"};
        nativeIMGsList[413] = {"NCOLUMN1.IMG", "PAL.COL", "0"};
        nativeIMGsList[414] = {"NCOLUMN2.IMG", "PAL.COL", "0"};
        nativeIMGsList[415] = {"NCOLUMN3.IMG", "PAL.COL", "0"};
        nativeIMGsList[416] = {"NCOLUMN4.IMG", "PAL.COL", "0"};
        nativeIMGsList[417] = {"NCOLUMN5.IMG", "PAL.COL", "0"};
        nativeIMGsList[418] = {"NCOLUMN6.IMG", "PAL.COL", "0"};
        nativeIMGsList[419] = {"NDENT1.IMG", "PAL.COL", "0"};
        nativeIMGsList[420] = {"NEGOTBUT.IMG", "PAL.COL", "8"};
        nativeIMGsList[421] = {"NEWEQUIP.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[422] = {"NEWMENU.IMG", "PAL.COL", "8"};
        nativeIMGsList[423] = {"NEWOLD.IMG", "PAL.COL", "8"};
        nativeIMGsList[424] = {"NEWPOP.IMG", "PAL.COL", "0"};
        nativeIMGsList[425] = {"NEW_CTR.IMG", "PAL.COL", "8"};
        nativeIMGsList[426] = {"NEW_LFT.IMG", "PAL.COL", "8"};
        nativeIMGsList[427] = {"NEW_RT.IMG", "PAL.COL", "8"};
        nativeIMGsList[428] = {"NFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[429] = {"NFIR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[430] = {"NFIR2.IMG", "PAL.COL", "0"};
        nativeIMGsList[431] = {"NGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[432] = {"NGRASSD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[433] = {"NGRATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[434] = {"NGRATED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[435] = {"NGRAVE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[436] = {"NGRAVE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[437] = {"NGRAVE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[438] = {"NGRAVE4.IMG", "PAL.COL", "0"};
        nativeIMGsList[439] = {"NHAYM.IMG", "PAL.COL", "0"};
        nativeIMGsList[440] = {"NHAYT.IMG", "PAL.COL", "0"};
        nativeIMGsList[441] = {"NHEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[442] = {"NHEDGED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[443] = {"NLAMP1DA.IMG", "PAL.COL", "0"};
        nativeIMGsList[444] = {"NOAK1.IMG", "PAL.COL", "0"};
        nativeIMGsList[445] = {"NOAK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[446] = {"NOAK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[447] = {"NOAK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[448] = {"NOBB.IMG", "PAL.COL", "texture"};
        nativeIMGsList[449] = {"NOB_A1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[450] = {"NOB_A2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[451] = {"NOB_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[452] = {"NOB_C1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[453] = {"NOB_C2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[454] = {"NOB_C3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[455] = {"NOB_D1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[456] = {"NOB_D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[457] = {"NOCAMP.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[458] = {"NOEXIT.IMG", "TAMRIEL.MNU", "8"};
        nativeIMGsList[459] = {"NOSPELL.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[460] = {"NPINE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[461] = {"NPINE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[462] = {"NPINE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[463] = {"NPINE4.IMG", "PAL.COL", "0"};
        nativeIMGsList[464] = {"NPOST11.IMG", "PAL.COL", "0"};
        nativeIMGsList[465] = {"NROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[466] = {"NROADD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[467] = {"NROCK1.IMG", "PAL.COL", "0"};
        nativeIMGsList[468] = {"NROCK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[469] = {"NROCK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[470] = {"NROCK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[471] = {"NROCK_O1.IMG", "PAL.COL", "0"};
        nativeIMGsList[472] = {"NROCK_O2.IMG", "PAL.COL", "0"};
        nativeIMGsList[473] = {"NROCK_O3.IMG", "PAL.COL", "0"};
        nativeIMGsList[474] = {"NROCK_O4.IMG", "PAL.COL", "0"};
        nativeIMGsList[475] = {"NRUINS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[476] = {"NRUINS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[477] = {"NRUINS3.IMG", "PAL.COL", "0"};
        nativeIMGsList[478] = {"NRUINS4.IMG", "PAL.COL", "0"};
        nativeIMGsList[479] = {"NRUINS5.IMG", "PAL.COL", "0"};
        nativeIMGsList[480] = {"NSIDWALD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[481] = {"NSIDWALK.IMG", "PAL.COL", "texture"};
        nativeIMGsList[482] = {"NSTATUE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[483] = {"NSTATUE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[484] = {"NSTATUE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[485] = {"NTOWN.IMG", "PAL.COL", "0"};
        nativeIMGsList[486] = {"NWEED1.IMG", "PAL.COL", "0"};
        nativeIMGsList[487] = {"NWEED2.IMG", "PAL.COL", "0"};
        nativeIMGsList[488] = {"NWELL1.IMG", "PAL.COL", "0"};
        nativeIMGsList[489] = {"NWELL2.IMG", "PAL.COL", "0"};
        nativeIMGsList[490] = {"OP.IMG", "PAL.COL", "0"};
        nativeIMGsList[491] = {"OVEN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[492] = {"P1.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[493] = {"PAGE2.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[494] = {"PALM1.IMG", "PAL.COL", "0"};
        nativeIMGsList[495] = {"PALM2.IMG", "PAL.COL", "0"};
        nativeIMGsList[496] = {"PALM3.IMG", "PAL.COL", "0"};
        nativeIMGsList[497] = {"PARCH.IMG", "Integrated", "1"};
        nativeIMGsList[498] = {"PARCHFLT.IMG", "PAL.COL", "0"};
        nativeIMGsList[499] = {"PGATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[500] = {"PILL11.IMG", "PAL.COL", "0"};
        nativeIMGsList[501] = {"PILL13.IMG", "PAL.COL", "0"};
        nativeIMGsList[502] = {"PILL15.IMG", "PAL.COL", "0"};
        nativeIMGsList[503] = {"PILL21.IMG", "PAL.COL", "0"};
        nativeIMGsList[504] = {"PILL23.IMG", "PAL.COL", "0"};
        nativeIMGsList[505] = {"PILL25.IMG", "PAL.COL", "0"};
        nativeIMGsList[506] = {"PILLOW01.IMG", "PAL.COL", "0"};
        nativeIMGsList[507] = {"PILOBONS.IMG", "PAL.COL", "0"};
        nativeIMGsList[508] = {"PIT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[509] = {"PLANT01.IMG", "PAL.COL", "0"};
        nativeIMGsList[510] = {"PLANT02.IMG", "PAL.COL", "0"};
        nativeIMGsList[511] = {"PLANT03.IMG", "PAL.COL", "0"};
        nativeIMGsList[512] = {"PLANT04.IMG", "PAL.COL", "0"};
        nativeIMGsList[513] = {"PLANT05.IMG", "PAL.COL", "0"};
        nativeIMGsList[514] = {"PLANT06.IMG", "PAL.COL", "0"};
        nativeIMGsList[515] = {"PLANT07.IMG", "PAL.COL", "0"};
        nativeIMGsList[516] = {"PLANT08.IMG", "PAL.COL", "0"};
        nativeIMGsList[517] = {"PLANT09.IMG", "PAL.COL", "0"};
        nativeIMGsList[518] = {"PLANT1.IMG", "PAL.COL", "0"};
        nativeIMGsList[519] = {"PLANT10.IMG", "PAL.COL", "0"};
        nativeIMGsList[520] = {"PLANT11.IMG", "PAL.COL", "0"};
        nativeIMGsList[521] = {"PLANT12.IMG", "PAL.COL", "0"};
        nativeIMGsList[522] = {"PLANT13.IMG", "PAL.COL", "0"};
        nativeIMGsList[523] = {"PLANT2.IMG", "PAL.COL", "0"};
        nativeIMGsList[524] = {"POINTER.IMG", "AUTOMAP.IMG", "0"};
        nativeIMGsList[525] = {"POPTALK.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[526] = {"POPUP.IMG", "CHARSPEL.IMG", "0"};
        nativeIMGsList[527] = {"POPUP11.IMG", "PAL.COL", "0"};
        nativeIMGsList[528] = {"POPUP2.IMG", "STARTGAM.MNU", "8"};
        nativeIMGsList[529] = {"POPUP3.IMG", "PAL.COL", "0"};
        nativeIMGsList[530] = {"POPUP4.IMG", "PAL.COL", "0"};
        nativeIMGsList[531] = {"POPUP5.IMG", "PAL.COL", "0"};
        nativeIMGsList[532] = {"POPUP7.IMG", "PAL.COL", "8"};
        nativeIMGsList[533] = {"POPUP8.IMG", "VALNWOOD.IMG", "8"};
        nativeIMGsList[534] = {"PORTCULS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[535] = {"PRINCE.IMG", "PAL.COL", "0"};
        nativeIMGsList[536] = {"PRINCESS.IMG", "PAL.COL", "0"};
        nativeIMGsList[537] = {"PUDD2.IMG", "PAL.COL", "0"};
        nativeIMGsList[538] = {"QUEEN.IMG", "PAL.COL", "0"};
        nativeIMGsList[539] = {"QUOTE.IMG", "Integrated", "1"};
        nativeIMGsList[540] = {"RBRIDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[541] = {"RBRIDGED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[542] = {"REDRUG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[543] = {"RFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[544] = {"RFIELDD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[545] = {"RGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[546] = {"RGRASSD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[547] = {"RGRATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[548] = {"RGRATED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[549] = {"RIVER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[550] = {"ROOTS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[551] = {"ROOTS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[552] = {"ROOTS3.IMG", "PAL.COL", "0"};
        nativeIMGsList[553] = {"RROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[554] = {"RROADD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[555] = {"RSIDWALD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[556] = {"RSIDWALK.IMG", "PAL.COL", "texture"};
        nativeIMGsList[557] = {"RUBBLE.IMG", "PAL.COL", "0"};
        nativeIMGsList[558] = {"S2.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[559] = {"SALTAR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[560] = {"SBAGAGE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[561] = {"SBARREL.IMG", "PAL.COL", "0"};
        nativeIMGsList[562] = {"SBONES1.IMG", "PAL.COL", "0"};
        nativeIMGsList[563] = {"SBOULDR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[564] = {"SBOULDR2.IMG", "PAL.COL", "0"};
        nativeIMGsList[565] = {"SBOULDR3.IMG", "PAL.COL", "0"};
        nativeIMGsList[566] = {"SBOULDR4.IMG", "PAL.COL", "0"};
        nativeIMGsList[567] = {"SBOX.IMG", "PAL.COL", "0"};
        nativeIMGsList[568] = {"SBRIDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[569] = {"SBUSH1.IMG", "PAL.COL", "0"};
        nativeIMGsList[570] = {"SBUSH2.IMG", "PAL.COL", "0"};
        nativeIMGsList[571] = {"SBUSH3.IMG", "PAL.COL", "0"};
        nativeIMGsList[572] = {"SBUSH4.IMG", "PAL.COL", "0"};
        nativeIMGsList[573] = {"SBUSH5.IMG", "PAL.COL", "0"};
        nativeIMGsList[574] = {"SBUSH6.IMG", "PAL.COL", "0"};
        nativeIMGsList[575] = {"SBUSH7.IMG", "PAL.COL", "0"};
        nativeIMGsList[576] = {"SBUSH8.IMG", "PAL.COL", "0"};
        nativeIMGsList[577] = {"SCART1.IMG", "PAL.COL", "0"};
        nativeIMGsList[578] = {"SCELL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[579] = {"SCOLUMN1.IMG", "PAL.COL", "0"};
        nativeIMGsList[580] = {"SCOLUMN2.IMG", "PAL.COL", "0"};
        nativeIMGsList[581] = {"SCOLUMN3.IMG", "PAL.COL", "0"};
        nativeIMGsList[582] = {"SCOLUMN4.IMG", "PAL.COL", "0"};
        nativeIMGsList[583] = {"SCOLUMN5.IMG", "PAL.COL", "0"};
        nativeIMGsList[584] = {"SCOLUMN6.IMG", "PAL.COL", "0"};
        nativeIMGsList[585] = {"SCROLL01.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[586] = {"SCROLL02.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[587] = {"SCROLL03.IMG", "SCROLL03.IMG", "0"};
        nativeIMGsList[588] = {"SCROLL1.IMG", "PAL.COL", "0"};
        nativeIMGsList[589] = {"SCROLL2.IMG", "PAL.COL", "0"};
        nativeIMGsList[590] = {"SDENT1.IMG", "PAL.COL", "0"};
        nativeIMGsList[591] = {"SEAWALL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[592] = {"SEAWALLD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[593] = {"SFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[594] = {"SFIR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[595] = {"SFIR2.IMG", "PAL.COL", "0"};
        nativeIMGsList[596] = {"SFOUNT1M.IMG", "PAL.COL", "0"};
        nativeIMGsList[597] = {"SFOUNT1T.IMG", "PAL.COL", "0"};
        nativeIMGsList[598] = {"SGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[599] = {"SGRATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[600] = {"SGRAVE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[601] = {"SGRAVE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[602] = {"SGRAVE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[603] = {"SGRAVE4.IMG", "PAL.COL", "0"};
        nativeIMGsList[604] = {"SHAYM.IMG", "PAL.COL", "0"};
        nativeIMGsList[605] = {"SHAYT.IMG", "PAL.COL", "0"};
        nativeIMGsList[606] = {"SHEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[607] = {"SHELF1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[608] = {"SHELF2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[609] = {"SKEL01.IMG", "PAL.COL", "0"};
        nativeIMGsList[610] = {"SKEL02.IMG", "PAL.COL", "0"};
        nativeIMGsList[611] = {"SKEL03.IMG", "PAL.COL", "0"};
        nativeIMGsList[612] = {"SKULL.IMG", "Integrated", "1"};
        nativeIMGsList[613] = {"SKYRIM.IMG", "Integrated", "1"};
        nativeIMGsList[614] = {"SLAMP1DA.IMG", "PAL.COL", "0"};
        nativeIMGsList[615] = {"SLAVA.IMG", "PAL.COL", "texture"};
        nativeIMGsList[616] = {"SLIDER.IMG", "PAL.COL", "noHeader"};
        nativeIMGsList[617] = {"SLIDSID.IMG", "PAL.COL", "0"};
        nativeIMGsList[618] = {"SLIDUP.IMG", "PAL.COL", "0"};
        nativeIMGsList[619] = {"SOAK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[620] = {"SOAK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[621] = {"SOAK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[622] = {"SPELL.IMG", "PAL.COL", "8"};
        nativeIMGsList[623] = {"SPELLBK.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[624] = {"SPELLMKR.IMG", "Integrated", "1"};
        nativeIMGsList[625] = {"SPINE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[626] = {"SPINE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[627] = {"SPINE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[628] = {"SPINE4.IMG", "PAL.COL", "0"};
        nativeIMGsList[629] = {"SPOST11.IMG", "PAL.COL", "0"};
        nativeIMGsList[630] = {"SROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[631] = {"SROCK1.IMG", "PAL.COL", "0"};
        nativeIMGsList[632] = {"SROCK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[633] = {"SROCK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[634] = {"SROCK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[635] = {"SROCK_O1.IMG", "PAL.COL", "0"};
        nativeIMGsList[636] = {"SROCK_O2.IMG", "PAL.COL", "0"};
        nativeIMGsList[637] = {"SROCK_O3.IMG", "PAL.COL", "0"};
        nativeIMGsList[638] = {"SROCK_O4.IMG", "PAL.COL", "0"};
        nativeIMGsList[639] = {"SRUINS1.IMG", "PAL.COL", "0"};
        nativeIMGsList[640] = {"SRUINS2.IMG", "PAL.COL", "0"};
        nativeIMGsList[641] = {"SRUINS3.IMG", "PAL.COL", "0"};
        nativeIMGsList[642] = {"SRUINS4.IMG", "PAL.COL", "0"};
        nativeIMGsList[643] = {"SRUINS5.IMG", "PAL.COL", "0"};
        nativeIMGsList[644] = {"SSD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[645] = {"SSIDWALK.IMG", "PAL.COL", "texture"};
        nativeIMGsList[646] = {"SSTATUE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[647] = {"SSTATUE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[648] = {"SSU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[649] = {"STAIRDN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[650] = {"STAIRDWN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[651] = {"STAIRUP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[652] = {"STAR1.IMG", "PAL.COL", "0"};
        nativeIMGsList[653] = {"STAR2.IMG", "PAL.COL", "0"};
        nativeIMGsList[654] = {"STAR3.IMG", "PAL.COL", "0"};
        nativeIMGsList[655] = {"STAR4.IMG", "PAL.COL", "0"};
        nativeIMGsList[656] = {"STAR5.IMG", "PAL.COL", "0"};
        nativeIMGsList[657] = {"STAR6.IMG", "PAL.COL", "0"};
        nativeIMGsList[658] = {"STAR7.IMG", "PAL.COL", "0"};
        nativeIMGsList[659] = {"STAR8.IMG", "PAL.COL", "0"};
        nativeIMGsList[660] = {"STARTGAM.IMG", "Integrated", "1"};
        nativeIMGsList[661] = {"STAT11.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[662] = {"STAT13.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[663] = {"STAT15.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[664] = {"STAT21.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[665] = {"STAT23.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[666] = {"STAT25.IMG", "CHARSHT.COL", "0"};
        nativeIMGsList[667] = {"STOWN.IMG", "PAL.COL", "0"};
        nativeIMGsList[668] = {"STRDWN2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[669] = {"STRUP2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[670] = {"SUMERSET.IMG", "Integrated", "1"};
        nativeIMGsList[671] = {"SUN.IMG", "PAL.COL", "8"};
        nativeIMGsList[672] = {"SWATER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[673] = {"SWEED1.IMG", "PAL.COL", "0"};
        nativeIMGsList[674] = {"SWEED2.IMG", "PAL.COL", "0"};
        nativeIMGsList[675] = {"SWELL1.IMG", "PAL.COL", "0"};
        nativeIMGsList[676] = {"SWELL2.IMG", "PAL.COL", "0"};
        nativeIMGsList[677] = {"TABLE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[678] = {"TABLE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[679] = {"TABLE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[680] = {"TABLET.IMG", "PAL.COL", "0"};
        nativeIMGsList[681] = {"TABSTN1.IMG", "PAL.COL", "0"};
        nativeIMGsList[682] = {"TABSTN3.IMG", "PAL.COL", "0"};
        nativeIMGsList[683] = {"TABSTN5.IMG", "PAL.COL", "0"};
        nativeIMGsList[684] = {"TAMRIEL.IMG", "Integrated", "1"};
        nativeIMGsList[685] = {"TAV_A1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[686] = {"TAV_A2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[687] = {"TAV_A3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[688] = {"TAV_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[689] = {"TAV_B2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[690] = {"TAV_B3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[691] = {"TAV_DOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[692] = {"TBK01COL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[693] = {"TBK01PAL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[694] = {"TBK02PAL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[695] = {"TEMP01.IMG", "PAL.COL", "8"};
        nativeIMGsList[696] = {"TEMP02.IMG", "PAL.COL", "8"};
        nativeIMGsList[697] = {"TEMP03.IMG", "PAL.COL", "8"};
        nativeIMGsList[698] = {"TEMP04.IMG", "PAL.COL", "8"};
        nativeIMGsList[699] = {"TEMP05.IMG", "PAL.COL", "8"};
        nativeIMGsList[700] = {"TEMP06.IMG", "PAL.COL", "8"};
        nativeIMGsList[701] = {"TEMP07.IMG", "PAL.COL", "8"};
        nativeIMGsList[702] = {"TEMP08.IMG", "PAL.COL", "8"};
        nativeIMGsList[703] = {"TEMP09.IMG", "PAL.COL", "8"};
        nativeIMGsList[704] = {"TEMP10.IMG", "PAL.COL", "8"};
        nativeIMGsList[705] = {"TEP_1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[706] = {"TEP_2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[707] = {"TEP_3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[708] = {"TEP_DOOR.IMG", "PAL.COL", "texture"};
        nativeIMGsList[709] = {"TEQ.IMG", "PAL.COL", "texture"};
        nativeIMGsList[710] = {"TEQD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[711] = {"TERRAIN.IMG", "TAMRIEL.MNU", "0"};
        nativeIMGsList[712] = {"TEST.IMG", "Integrated", "1"};
        nativeIMGsList[713] = {"THRUSH01.IMG", "PAL.COL", "0"};
        nativeIMGsList[714] = {"THRUSH02.IMG", "PAL.COL", "0"};
        nativeIMGsList[715] = {"TLAUNDR1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[716] = {"TLAUNDR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[717] = {"TLAUNDRY.IMG", "PAL.COL", "texture"};
        nativeIMGsList[718] = {"TMU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[719] = {"TMUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[720] = {"TOP.IMG", "CHARSHT.COL", "8"};
        nativeIMGsList[721] = {"TORCH1.IMG", "PAL.COL", "0"};
        nativeIMGsList[722] = {"TORCH3.IMG", "PAL.COL", "0"};
        nativeIMGsList[723] = {"TORCH5.IMG", "PAL.COL", "0"};
        nativeIMGsList[724] = {"TOWER.IMG", "Integrated", "1"};
        nativeIMGsList[725] = {"TOWN.IMG", "VALNWOOD.IMG", "noHeader"};
        nativeIMGsList[726] = {"TREASRE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[727] = {"TREASRE2.IMG", "PAL.COL", "0"};
        nativeIMGsList[728] = {"TREASRE3.IMG", "PAL.COL", "0"};
        nativeIMGsList[729] = {"TREE02.IMG", "PAL.COL", "0"};
        nativeIMGsList[730] = {"TREE03.IMG", "PAL.COL", "0"};
        nativeIMGsList[731] = {"TREE04.IMG", "PAL.COL", "0"};
        nativeIMGsList[732] = {"TREE05.IMG", "PAL.COL", "0"};
        nativeIMGsList[733] = {"TREE06.IMG", "PAL.COL", "0"};
        nativeIMGsList[734] = {"TREE07.IMG", "PAL.COL", "0"};
        nativeIMGsList[735] = {"TREE08.IMG", "PAL.COL", "0"};
        nativeIMGsList[736] = {"TREE1.IMG", "PAL.COL", "0"};
        nativeIMGsList[737] = {"TREECEIL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[738] = {"TSCARY01.IMG", "PAL.COL", "0"};
        nativeIMGsList[739] = {"TSCARY02.IMG", "PAL.COL", "0"};
        nativeIMGsList[740] = {"TSCARY03.IMG", "PAL.COL", "0"};
        nativeIMGsList[741] = {"TSCARY04.IMG", "PAL.COL", "0"};
        nativeIMGsList[742] = {"TTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[743] = {"TTAVD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[744] = {"TTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[745] = {"TTEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[746] = {"TTOWER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[747] = {"TZCITYWL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[748] = {"TZDBARN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[749] = {"TZDCRYPT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[750] = {"TZDFARM.IMG", "PAL.COL", "texture"};
        nativeIMGsList[751] = {"TZDLGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[752] = {"TZDRGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[753] = {"TZDTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[754] = {"TZDTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[755] = {"TZDTOWER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[756] = {"TZGARDEN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[757] = {"TZNBRIDG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[758] = {"TZNFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[759] = {"TZNGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[760] = {"TZNHEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[761] = {"TZNROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[762] = {"TZRBRIDG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[763] = {"TZRFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[764] = {"TZRGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[765] = {"TZRIVER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[766] = {"TZRROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[767] = {"TZSBRIDG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[768] = {"TZSFIELD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[769] = {"TZSGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[770] = {"TZSHEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[771] = {"TZSROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[772] = {"TZTTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[773] = {"TZTTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[774] = {"TZTTOWER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[775] = {"T_CITYWL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[776] = {"T_DBS1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[777] = {"T_DBS2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[778] = {"T_DBS3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[779] = {"T_DEQ.IMG", "PAL.COL", "texture"};
        nativeIMGsList[780] = {"T_DLGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[781] = {"T_DMU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[782] = {"T_DNB1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[783] = {"T_DRGT.IMG", "PAL.COL", "texture"};
        nativeIMGsList[784] = {"T_DTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[785] = {"T_DTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[786] = {"T_GARDEN.IMG", "PAL.COL", "texture"};
        nativeIMGsList[787] = {"T_NBRIDG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[788] = {"T_NGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[789] = {"T_NGRATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[790] = {"T_NHEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[791] = {"T_NROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[792] = {"T_NSDWLK.IMG", "PAL.COL", "texture"};
        nativeIMGsList[793] = {"T_RBRIDG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[794] = {"T_RGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[795] = {"T_RGRATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[796] = {"T_RROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[797] = {"T_RSDWLK.IMG", "PAL.COL", "texture"};
        nativeIMGsList[798] = {"T_SBRIDG.IMG", "PAL.COL", "texture"};
        nativeIMGsList[799] = {"T_SEAWAL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[800] = {"T_SGRASS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[801] = {"T_SGRATE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[802] = {"T_SHEDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[803] = {"T_SROAD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[804] = {"T_SSDWLK.IMG", "PAL.COL", "texture"};
        nativeIMGsList[805] = {"T_TEQ.IMG", "PAL.COL", "texture"};
        nativeIMGsList[806] = {"T_TMU.IMG", "PAL.COL", "texture"};
        nativeIMGsList[807] = {"T_TTAV.IMG", "PAL.COL", "texture"};
        nativeIMGsList[808] = {"T_TTEP.IMG", "PAL.COL", "texture"};
        nativeIMGsList[809] = {"UPDOWN.IMG", "CHARSHT.COL", "noHeader"};
        nativeIMGsList[810] = {"VALNWOOD.IMG", "Integrated", "1"};
        nativeIMGsList[811] = {"VASE.IMG", "PAL.COL", "0"};
        nativeIMGsList[812] = {"VILLAGE.IMG", "VALNWOOD.IMG", "noHeader"};
        nativeIMGsList[813] = {"VINE01.IMG", "PAL.COL", "0"};
        nativeIMGsList[814] = {"VINE02.IMG", "PAL.COL", "0"};
        nativeIMGsList[815] = {"VINE03.IMG", "PAL.COL", "0"};
        nativeIMGsList[816] = {"VINE04.IMG", "PAL.COL", "0"};
        nativeIMGsList[817] = {"VOLCANO3.IMG", "PAL.COL", "8"};
        nativeIMGsList[818] = {"W01BS1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[819] = {"W01BS2D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[820] = {"W01EQD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[821] = {"W01MUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[822] = {"W01TAVD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[823] = {"W01TEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[824] = {"W02BS1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[825] = {"W02BS2D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[826] = {"W02EQD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[827] = {"W02MUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[828] = {"W02TAVD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[829] = {"W02TEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[830] = {"W03BS1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[831] = {"W03EQD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[832] = {"W03MUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[833] = {"W03TAVD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[834] = {"W03TEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[835] = {"W04BS1D.IMG", "PAL.COL", "texture"};
        nativeIMGsList[836] = {"W04MUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[837] = {"W04TEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[838] = {"W05MUD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[839] = {"W05TEPD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[840] = {"WALL_A01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[841] = {"WALL_A02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[842] = {"WALL_A03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[843] = {"WALL_A04.IMG", "PAL.COL", "texture"};
        nativeIMGsList[844] = {"WALL_BO1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[845] = {"WALL_BO2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[846] = {"WALL_BO3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[847] = {"WALL_C01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[848] = {"WALL_C02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[849] = {"WALL_D05.IMG", "PAL.COL", "texture"};
        nativeIMGsList[850] = {"WALL_D06.IMG", "PAL.COL", "texture"};
        nativeIMGsList[851] = {"WALL_DO1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[852] = {"WALL_DO2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[853] = {"WALL_DO3.IMG", "PAL.COL", "texture"};
        nativeIMGsList[854] = {"WALL_DO4.IMG", "PAL.COL", "texture"};
        nativeIMGsList[855] = {"WALL_E01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[856] = {"WALL_E02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[857] = {"WALL_E03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[858] = {"WALL_E04.IMG", "PAL.COL", "texture"};
        nativeIMGsList[859] = {"WALL_F01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[860] = {"WALL_F02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[861] = {"WALL_F03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[862] = {"WALL_F04.IMG", "PAL.COL", "texture"};
        nativeIMGsList[863] = {"WALL_G01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[864] = {"WALL_G02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[865] = {"WALL_G03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[866] = {"WALL_H01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[867] = {"WALL_H02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[868] = {"WALL_H03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[869] = {"WALL_H04.IMG", "PAL.COL", "texture"};
        nativeIMGsList[870] = {"WALL_I01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[871] = {"WALL_I02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[872] = {"WALL_I03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[873] = {"WALL_I04.IMG", "PAL.COL", "texture"};
        nativeIMGsList[874] = {"WALL_J01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[875] = {"WALL_J02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[876] = {"WALL_J03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[877] = {"WALL_K01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[878] = {"WALL_K02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[879] = {"WALL_K03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[880] = {"WALL_L01.IMG", "PAL.COL", "texture"};
        nativeIMGsList[881] = {"WALL_L02.IMG", "PAL.COL", "texture"};
        nativeIMGsList[882] = {"WALL_L03.IMG", "PAL.COL", "texture"};
        nativeIMGsList[883] = {"WARDROBE.IMG", "PAL.COL", "0"};
        nativeIMGsList[884] = {"WATER.IMG", "PAL.COL", "texture"};
        nativeIMGsList[885] = {"WATERWAL.IMG", "PAL.COL", "texture"};
        nativeIMGsList[886] = {"WCAS.IMG", "PAL.COL", "texture"};
        nativeIMGsList[887] = {"WDNG2_A1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[888] = {"WDNG2_A2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[889] = {"WDNG2_B1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[890] = {"WDNG2_B2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[891] = {"WDNG2_C1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[892] = {"WDNG2_C2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[893] = {"WDNG2_D1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[894] = {"WDNG2_D2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[895] = {"WDNG2_E1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[896] = {"WDNG2_E2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[897] = {"WDNG2_F1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[898] = {"WDNG2_F2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[899] = {"WDNG2_G1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[900] = {"WDNG2_G2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[901] = {"WDNG2_H1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[902] = {"WDNG2_H2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[903] = {"WDNG2_I1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[904] = {"WDNG2_I2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[905] = {"WDNG2_J1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[906] = {"WDNG2_J2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[907] = {"WDNG2_K1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[908] = {"WDNG2_K2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[909] = {"WDNG2_L1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[910] = {"WDNG2_L2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[911] = {"WDNG2_M1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[912] = {"WDNG2_M2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[913] = {"WDNG2_N1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[914] = {"WDNG2_N2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[915] = {"WDNG2_O1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[916] = {"WDNG2_O2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[917] = {"WDNG2_P1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[918] = {"WDNG2_P2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[919] = {"WDNG2_Q1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[920] = {"WDNG2_Q2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[921] = {"WDNG2_R1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[922] = {"WDNG2_R2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[923] = {"WDNG2_S1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[924] = {"WDNG2_S2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[925] = {"WDUNGDR1.IMG", "PAL.COL", "texture"};
        nativeIMGsList[926] = {"WDUNGDR2.IMG", "PAL.COL", "texture"};
        nativeIMGsList[927] = {"WOAK2.IMG", "PAL.COL", "0"};
        nativeIMGsList[928] = {"WOAK3.IMG", "PAL.COL", "0"};
        nativeIMGsList[929] = {"WOAK4.IMG", "PAL.COL", "0"};
        nativeIMGsList[930] = {"WOMAN01.IMG", "PAL.COL", "0"};
        nativeIMGsList[931] = {"WOMAN02.IMG", "PAL.COL", "0"};
        nativeIMGsList[932] = {"WOMAN03.IMG", "PAL.COL", "0"};
        nativeIMGsList[933] = {"WOMAN04.IMG", "PAL.COL", "0"};
        nativeIMGsList[934] = {"WRITING1.IMG", "PAL.COL", "0"};
        nativeIMGsList[935] = {"XARDENGD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[936] = {"XDLGTD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[937] = {"XDRGTD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[938] = {"XNBRIDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[939] = {"XNGRASSD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[940] = {"XNHEDGED.IMG", "PAL.COL", "texture"};
        nativeIMGsList[941] = {"XNROADD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[942] = {"XRBRIDGE.IMG", "PAL.COL", "texture"};
        nativeIMGsList[943] = {"XRGRASSD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[944] = {"XRROADD.IMG", "PAL.COL", "texture"};
        nativeIMGsList[945] = {"YESNO.IMG", "PAL.COL", "8"};
        nativeIMGsList[946] = {"TITLE.IMG", "Integrated", "1"};
    }

    // List of all Arena native SET files and their dimensions
    {
        nativeSETsList[0] = {"CASA.SET", 64, 256};
        nativeSETsList[1] = {"CASB.SET", 64, 192};
        nativeSETsList[2] = {"CASC.SET", 64, 128};
        nativeSETsList[3] = {"CASD.SET", 64, 256};
        nativeSETsList[4] = {"CASE.SET", 64, 256};
        nativeSETsList[5] = {"CASF.SET", 64, 256};
        nativeSETsList[6] = {"CASG.SET", 64, 192};
        nativeSETsList[7] = {"CASH.SET", 64, 256};
        nativeSETsList[8] = {"CASI.SET", 64, 256};
        nativeSETsList[9] = {"CASJ.SET", 64, 192};
        nativeSETsList[10] = {"CASK.SET", 64, 192};
        nativeSETsList[11] = {"CASL.SET", 64, 128};
        nativeSETsList[12] = {"DMBS1.SET", 64, 256};
        nativeSETsList[13] = {"DMBS2.SET", 64, 256};
        nativeSETsList[14] = {"DMBS3.SET", 64, 192};
        nativeSETsList[15] = {"DMEQUIP.SET", 64, 192};
        nativeSETsList[16] = {"DMMUGUIL.SET", 64, 320};
        nativeSETsList[17] = {"DMNOBLE.SET", 64, 192};
        nativeSETsList[18] = {"DMTAVERN.SET", 64, 192};
        nativeSETsList[19] = {"DMTEMPLE.SET", 64, 320};
        nativeSETsList[20] = {"DWA01.SET", 64, 128};
        nativeSETsList[21] = {"DWA02.SET", 64, 128};
        nativeSETsList[22] = {"DWB01.SET", 64, 128};
        nativeSETsList[23] = {"DWB02.SET", 64, 128};
        nativeSETsList[24] = {"DWBARN.SET", 64, 128};
        nativeSETsList[25] = {"DWC01.SET", 64, 128};
        nativeSETsList[26] = {"DWC02.SET", 64, 128};
        nativeSETsList[27] = {"DWCORN.SET", 64, 128};
        nativeSETsList[28] = {"DWCRYPT.SET", 64, 256};
        nativeSETsList[29] = {"DWD01.SET", 64, 128};
        nativeSETsList[30] = {"DWD02.SET", 64, 128};
        nativeSETsList[31] = {"DWE01.SET", 64, 128};
        nativeSETsList[32] = {"DWE02.SET", 64, 128};
        nativeSETsList[33] = {"DWF01.SET", 64, 128};
        nativeSETsList[34] = {"DWF02.SET", 64, 128};
        nativeSETsList[35] = {"DWFARM.SET", 64, 192};
        nativeSETsList[36] = {"DWFENCE1.SET", 64, 192};
        nativeSETsList[37] = {"DWFENCE2.SET", 64, 192};
        nativeSETsList[38] = {"DWG01.SET", 64, 128};
        nativeSETsList[39] = {"DWG02.SET", 64, 128};
        nativeSETsList[40] = {"DWH01.SET", 64, 128};
        nativeSETsList[41] = {"DWH02.SET", 64, 128};
        nativeSETsList[42] = {"DWI01.SET", 64, 128};
        nativeSETsList[43] = {"DWI02.SET", 64, 128};
        nativeSETsList[44] = {"DWJ01.SET", 64, 128};
        nativeSETsList[45] = {"DWJ02.SET", 64, 128};
        nativeSETsList[46] = {"DWK01.SET", 64, 128};
        nativeSETsList[47] = {"DWK02.SET", 64, 128};
        nativeSETsList[48] = {"DWL01.SET", 64, 128};
        nativeSETsList[49] = {"DWL02.SET", 64, 128};
        nativeSETsList[50] = {"DWM01.SET", 64, 128};
        nativeSETsList[51] = {"DWM02.SET", 64, 128};
        nativeSETsList[52] = {"DWN01.SET", 64, 128};
        nativeSETsList[53] = {"DWN02.SET", 64, 128};
        nativeSETsList[54] = {"DWO01.SET", 64, 128};
        nativeSETsList[55] = {"DWO02.SET", 64, 128};
        nativeSETsList[56] = {"DWP01.SET", 64, 128};
        nativeSETsList[57] = {"DWP02.SET", 64, 128};
        nativeSETsList[58] = {"DWQ01.SET", 64, 128};
        nativeSETsList[59] = {"DWQ02.SET", 64, 128};
        nativeSETsList[60] = {"DWR01.SET", 64, 128};
        nativeSETsList[61] = {"DWR02.SET", 64, 128};
        nativeSETsList[62] = {"DWS01.SET", 64, 128};
        nativeSETsList[63] = {"DWS02.SET", 64, 128};
        nativeSETsList[64] = {"DWTAVERN.SET", 64, 192};
        nativeSETsList[65] = {"DWTEMPLE.SET", 64, 256};
        nativeSETsList[66] = {"DWTOWER.SET", 64, 384};
        nativeSETsList[67] = {"EQA.SET", 64, 192};
        nativeSETsList[68] = {"EQB.SET", 64, 256};
        nativeSETsList[69] = {"ERIE.SET", 64, 256};
        nativeSETsList[70] = {"FLCASC.SET", 64, 128};
        nativeSETsList[71] = {"FLCASD.SET", 64, 128};
        nativeSETsList[72] = {"FLOORA.SET", 64, 256};
        nativeSETsList[73] = {"FLOORB.SET", 64, 128};
        nativeSETsList[74] = {"FLOORC.SET", 64, 192};
        nativeSETsList[75] = {"FLOORD.SET", 64, 128};
        nativeSETsList[76] = {"ICEA.SET", 64, 128};
        nativeSETsList[77] = {"ICEB.SET", 64, 128};
        nativeSETsList[78] = {"ICEC.SET", 64, 192};
        nativeSETsList[79] = {"ICED.SET", 64, 192};
        nativeSETsList[80] = {"ICEE.SET", 64, 192};
        nativeSETsList[81] = {"ICEF.SET", 64, 192};
        nativeSETsList[82] = {"ICEG.SET", 64, 192};
        nativeSETsList[83] = {"ICEH.SET", 64, 128};
        nativeSETsList[84] = {"ICEI.SET", 64, 256};
        nativeSETsList[85] = {"ICEJ.SET", 64, 192};
        nativeSETsList[86] = {"ICEK.SET", 64, 256};
        nativeSETsList[87] = {"ICEL.SET", 64, 256};
        nativeSETsList[88] = {"ICEM.SET", 64, 256};
        nativeSETsList[89] = {"ICEN.SET", 64, 128};
        nativeSETsList[90] = {"INTA.SET", 64, 192};
        nativeSETsList[91] = {"INTB.SET", 64, 192};
        nativeSETsList[92] = {"INTC.SET", 64, 128};
        nativeSETsList[93] = {"MBS1.SET", 64, 256};
        nativeSETsList[94] = {"MBS2.SET", 64, 256};
        nativeSETsList[95] = {"MBS3.SET", 64, 192};
        nativeSETsList[96] = {"MEQUIP.SET", 64, 192};
        nativeSETsList[97] = {"MFLOORA.SET", 64, 128};
        nativeSETsList[98] = {"MFLOORD.SET", 64, 128};
        nativeSETsList[99] = {"MINEA.SET", 64, 256};
        nativeSETsList[100] = {"MINEA1.SET", 64, 256};
        nativeSETsList[101] = {"MINEB.SET", 64, 256};
        nativeSETsList[102] = {"MINEB1.SET", 64, 256};
        nativeSETsList[103] = {"MINEC.SET", 64, 256};
        nativeSETsList[104] = {"MINEC1.SET", 64, 256};
        nativeSETsList[105] = {"MINED.SET", 64, 256};
        nativeSETsList[106] = {"MINEE.SET", 64, 192};
        nativeSETsList[107] = {"MINEF.SET", 64, 128};
        nativeSETsList[108] = {"MMUGUILD.SET", 64, 320};
        nativeSETsList[109] = {"MNOBLE.SET", 64, 192};
        nativeSETsList[110] = {"MTAVERN.SET", 64, 192};
        nativeSETsList[111] = {"MTEMPLE.SET", 64, 320};
        nativeSETsList[112] = {"MU.SET", 64, 384};
        nativeSETsList[113] = {"MWBARN.SET", 64, 128};
        nativeSETsList[114] = {"MWCORN.SET", 64, 128};
        nativeSETsList[115] = {"MWCRYPT.SET", 64, 256};
        nativeSETsList[116] = {"MWFARM.SET", 64, 192};
        nativeSETsList[117] = {"MWFENCE1.SET", 64, 192};
        nativeSETsList[118] = {"MWFENCE2.SET", 64, 192};
        nativeSETsList[119] = {"MWTAVERN.SET", 64, 192};
        nativeSETsList[120] = {"MWTEMPLE.SET", 64, 256};
        nativeSETsList[121] = {"MWTOWER.SET", 64, 384};
        nativeSETsList[122] = {"NOBA.SET", 64, 128};
        nativeSETsList[123] = {"NOBC.SET", 64, 192};
        nativeSETsList[124] = {"NOBD.SET", 64, 128};
        nativeSETsList[125] = {"NORM1.SET", 64, 320};
        nativeSETsList[126] = {"NORM2.SET", 64, 320};
        nativeSETsList[127] = {"NORM3.SET", 64, 256};
        nativeSETsList[128] = {"NORM7.SET", 64, 320};
        nativeSETsList[129] = {"R2FLOORA.SET", 64, 192};
        nativeSETsList[130] = {"R2FLOORB.SET", 64, 192};
        nativeSETsList[131] = {"R2FLOORC.SET", 64, 192};
        nativeSETsList[132] = {"R2FLOORD.SET", 64, 192};
        nativeSETsList[133] = {"RAIN1.SET", 64, 320};
        nativeSETsList[134] = {"RAIN2.SET", 64, 320};
        nativeSETsList[135] = {"RAIN3.SET", 64, 256};
        nativeSETsList[136] = {"RAIN7.SET", 64, 320};
        nativeSETsList[137] = {"RFLOORA.SET", 64, 192};
        nativeSETsList[138] = {"RFLOORB.SET", 64, 192};
        nativeSETsList[139] = {"RFLOORC.SET", 64, 192};
        nativeSETsList[140] = {"RFLOORD.SET", 64, 192};
        nativeSETsList[141] = {"SHELVES.SET", 64, 256};
        nativeSETsList[142] = {"SNOW1.SET", 64, 320};
        nativeSETsList[143] = {"SNOW2.SET", 64, 320};
        nativeSETsList[144] = {"SNOW3.SET", 64, 256};
        nativeSETsList[145] = {"SNOW7.SET", 64, 320};
        nativeSETsList[146] = {"TAVA.SET", 64, 192};
        nativeSETsList[147] = {"TAVB.SET", 64, 192};
        nativeSETsList[148] = {"TBS1.SET", 64, 256};
        nativeSETsList[149] = {"TBS2.SET", 64, 255};
        nativeSETsList[150] = {"TBS3.SET", 64, 192};
        nativeSETsList[151] = {"TEPA.SET", 64, 192};
        nativeSETsList[152] = {"TEQUIP.SET", 64, 192};
        nativeSETsList[153] = {"TMUGUILD.SET", 64, 320};
        nativeSETsList[154] = {"TNOBLE.SET", 64, 192};
        nativeSETsList[155] = {"TREECEIL.SET", 64, 256};
        nativeSETsList[156] = {"TREEWAL2.SET", 64, 256};
        nativeSETsList[157] = {"TREEWALL.SET", 64, 256};
        nativeSETsList[158] = {"TTAVERN.SET", 64, 192};
        nativeSETsList[159] = {"TTEMPLE.SET", 64, 320};
        nativeSETsList[160] = {"TWBARN.SET", 64, 128};
        nativeSETsList[161] = {"TWCORN.SET", 64, 128};
        nativeSETsList[162] = {"TWCRYPT.SET", 64, 256};
        nativeSETsList[163] = {"TWFARM.SET", 64, 192};
        nativeSETsList[164] = {"TWFENCE1.SET", 64, 192};
        nativeSETsList[165] = {"TWFENCE2.SET", 64, 192};
        nativeSETsList[166] = {"TWTAVERN.SET", 64, 192};
        nativeSETsList[167] = {"TWTEMPLE.SET", 64, 256};
        nativeSETsList[168] = {"TWTOWER.SET", 64, 384};
        nativeSETsList[169] = {"WALLA.SET", 64, 320};
        nativeSETsList[170] = {"WALLB.SET", 64, 320};
        nativeSETsList[171] = {"WALLC.SET", 64, 320};
        nativeSETsList[172] = {"WALLD.SET", 64, 320};
        nativeSETsList[173] = {"WALLE.SET", 64, 192};
        nativeSETsList[174] = {"WALLF.SET", 64, 256};
        nativeSETsList[175] = {"WALLG.SET", 64, 256};
        nativeSETsList[176] = {"WALLH.SET", 64, 256};
        nativeSETsList[177] = {"WNORM1.SET", 64, 320};
        nativeSETsList[178] = {"WRAIN1.SET", 64, 320};
        nativeSETsList[179] = {"WSNOW1.SET", 64, 320};
    }

    // IMG with no header and size to set manually
    {
    noHeaderIMG[0] = {"ARENARW.IMG", 16, 16};
    noHeaderIMG[1] = {"CITY.IMG", 16, 11};
    noHeaderIMG[2] = {"DITHER.IMG", 16, 50};
    noHeaderIMG[3] = {"DITHER2.IMG", 16, 50};
    noHeaderIMG[4] = {"DUNGEON.IMG", 14, 8};
    noHeaderIMG[5] = {"DZTTAV.IMG", 32, 34};
    noHeaderIMG[6] = {"NOCAMP.IMG", 25, 19};
    noHeaderIMG[7] = {"NOSPELL.IMG", 25, 19};
    noHeaderIMG[8] = {"P1.IMG", 320, 53};
    noHeaderIMG[9] = {"POPTALK.IMG", 320, 77};
    noHeaderIMG[10] = {"S2.IMG", 320, 36};
    noHeaderIMG[11] = {"SLIDER.IMG", 289, 7};
    noHeaderIMG[12] = {"TOWN.IMG", 9, 10};
    noHeaderIMG[13] = {"UPDOWN.IMG", 8, 16};
    noHeaderIMG[14] = {"VILLAGE.IMG", 8, 8};
    }

    // Table of CIF file with raw data and dimension of frame to set manually
    {
    noHeaderCIF[0] = {"BRASS.CIF", 8, 8};
    noHeaderCIF[1] = {"BRASS2.CIF", 8, 8};
    noHeaderCIF[2] = {"MARBLE.CIF", 3, 3};
    noHeaderCIF[3] = {"MARBLE2.CIF", 3, 3};
    noHeaderCIF[4] = {"PARCH.CIF", 20, 20};
    noHeaderCIF[5] = {"SCROLL.CIF", 20, 20};
    }

    // Buffers
    imageData = new uchar[64000];
    compressedImageData = new uchar[64000];
}

// Destructor
Image::~Image()
{
    delete[] imageData;
    delete[] compressedImageData;
}

// Build a QImage from file index and return it
QImage Image::buildQImage(int index, std::string extension)
{
    std::ifstream *stream = BSAFile::getInstance()->getStream();
    // If file is a SET file
    if (extension == "SET")
    {
        size_t fileSize = BSAFile::getInstance()->getFileSize(index);
        width = 64;
        height = fileSize/64;
        flags = 0xFFFF;
        stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
        if (fileSize > 64000)
        {
            delete[] imageData;
            imageData = new uchar[fileSize];
        }
        stream->read(reinterpret_cast<char*>(imageData), fileSize);
        Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
        palette = "PAL.COL";
        Img.setColorTable(palettesMap[palette]);
        hasTransparency = false;
        return Img;
    }
    // If file is a IMG file
    else if (extension == "IMG")
    {
        // first check if IMG with has no header and size 64x64
        if (BSAFile::getInstance()->getFileSize(index) == 4096)
        {
            width = 64;
            height = 64;
            flags = 0xFFFF;
            stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
            stream->read(reinterpret_cast<char*>(imageData), BSAFile::getInstance()->getFileSize(index));
            Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
            Img.setColorTable(transparentPalettesMap["PAL.COL"]);
            hasTransparency = true;
            palette = "PAL.COL";
            return Img;
        }
        // then if IMG has no header but various size to set manually
        for (int i(0); i < 15; i++)
        {
            std::string fileName = BSAFile::getInstance()->getFileName(index);
            if (fileName == noHeaderIMG[i].fileName)
            {
                width = noHeaderIMG[i].width;
                height = noHeaderIMG[i].height;
                flags = 0xFFFF;
                stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
                stream->read(reinterpret_cast<char*>(imageData), BSAFile::getInstance()->getFileSize(index));
                Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                for (int i(0); i<947; i++)
                {
                    if (nativeIMGsList[i].fileName == fileName)
                    {
                        palette = nativeIMGsList[i].paletteName;
                        Img.setColorTable(transparentPalettesMap[palette]);
                        hasTransparency = true;
                        return Img;
                    }
                }
            }
        }
        // All other IMG sould have a header
        // Get value from the 12 bytes header
        stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
        stream->read(reinterpret_cast<char*>(&offsetX), 2);
        stream->read(reinterpret_cast<char*>(&offsetY), 2);
        stream->read(reinterpret_cast<char*>(&width), 2);
        stream->read(reinterpret_cast<char*>(&height), 2);
        stream->read(reinterpret_cast<char*>(&flags), 2);
        stream->read(reinterpret_cast<char*>(&dataSize), 2);
        // IMG compressed in 08 has the decompressed size stored after the header
        if ((flags & 0x00FF) == 0x0008)
        {
            stream->read(reinterpret_cast<char*>(&decompressedSize), 2);
        }
        // Set the buffers bigger if needed
        if (width*height > 64000 || dataSize > 64000)
        {
            delete[] imageData;
            imageData = new uchar[width*height];
            delete[] compressedImageData;
            compressedImageData = new uchar[dataSize];
        }
        // IMG not compressed
        if ((flags & 0x00FF) == 0x0000)
        {
            stream->read(reinterpret_cast<char*>(imageData), dataSize);
            Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
        }
        // IMG compressed in 04
        else if ((flags & 0x00FF) == 0x0004)
        {
            stream->read(reinterpret_cast<char*>(compressedImageData), dataSize);
            Compression::image04Decompression(compressedImageData, imageData, dataSize);
            Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
        }
        // IMG compressed in 08
        else if ((flags & 0x00FF) == 0x0008 && decompressedSize != 0)
        {
            stream->read(reinterpret_cast<char*>(compressedImageData), dataSize-2);
            Compression::image08Decompression(compressedImageData, imageData, dataSize-2, decompressedSize);
            Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
        }
        // Unknown IMG compression flag -> invalid IMG file
        else
        {
            Img = QImage(0, 0, QImage::Format_Indexed8);
            return Img;
        }
        // In case the IMG has an integrated palette
        if ((flags & 0x0100) == 0x0100)
        {
            uint8_t R(0), G(0), B(0);
            palette = "Integrated";
            palettesMap[palette].clear();
            // First color transparent or not
            stream->read(reinterpret_cast<char*>(&R), 1);
            stream->read(reinterpret_cast<char*>(&G), 1);
            stream->read(reinterpret_cast<char*>(&B), 1);
            if (R != 0)
            {
                R = (R << 2) + 3;
            }
            if (G != 0)
            {
                G = (G << 2) + 3;
            }
            if (B != 0)
            {
                B = (B << 2) + 3;
            }
            // Not transparent if integrated 01
            if ((flags & 0xFF00) == 0x0100)
            {
                palettesMap[palette].append(qRgb(R, G, B));
                hasTransparency = false;
            }
            // Transparent if integrated 09
            else
            {
                transparentPalettesMap[palette].append(qRgba(R, G, B, 1));
                hasTransparency = true;
            }
            // Other 255 color
            for (size_t i(1); i<256; i++)
            {
                stream->read(reinterpret_cast<char*>(&R), 1);
                stream->read(reinterpret_cast<char*>(&G), 1);
                stream->read(reinterpret_cast<char*>(&B), 1);
                // All integrated palette have a max of 0x3F for color value instead of 0xFF (value originally store in 6 bits instead of 8)
                // Goal is to lighter the palette by converting 6bit color to 8bit with a left bit shift of 2 and adding 3 (if not 0)
                if (R != 0)
                {
                    R = (R << 2) + 3;
                }
                if (G != 0)
                {
                    G = (G << 2) + 3;
                }
                if (B != 0)
                {
                    B = (B << 2) + 3;
                }
                if ((flags & 0xFF00) == 0x0100)
                {
                    palettesMap[palette].append(qRgb(R, G, B));
                }
                else
                {
                    transparentPalettesMap[palette].append(qRgb(R, G, B));
                }
            }
            // Not transparent
            if ((flags & 0xFF00) == 0x0100)
            {
                Img.setColorTable(palettesMap[palette]);
            }
            // Transparent
            else
            {
                Img.setColorTable(transparentPalettesMap[palette]);
            }
            return Img;
        }
        // Choose a color table if IMG has none integrated and is a native Arena IMG
        else
        {
            std::string fileName = BSAFile::getInstance()->getFileName(index);
            for (int i(0); i<947; i++)
            {
                if (nativeIMGsList[i].fileName == fileName)
                {
                    palette = nativeIMGsList[i].paletteName;
                    // If native palette is integrated but actual file palette flag is set to not integrated -> return image with PAL.COL
                    // If want to handle it differently, can be found after getting the QImage by getting flag not 01 or 09 but getting palette integrated
                    if (palette == "Integrated")
                    {
                        Img.setColorTable(palettesMap["PAL.COL"]);
                        hasTransparency = false;
                        return Img;
                    }
                    // Seting up palette with transparency
                    Img.setColorTable(transparentPalettesMap[palette]);
                    hasTransparency = true;
                    return Img;
                }
            }
        }
        // Finally set color table to PAL.COL with no tranparency if IMG is not a native Arena IMG but
        // format has been validated by the process with no integrated palette
        palette = "PAL.COL";
        Img.setColorTable(palettesMap[palette]);
        hasTransparency = false;
        return Img;
    }
    // If not IMG or SET file
    else
    {
        Img = QImage(0, 0, QImage::Format_Indexed8);
        return Img;
    }
}

// Overloaded function
// Build a QImage from file path and return it
QImage Image::buildQImage(const std::string filePath)
{
    int fileNamePos = filePath.find_last_of("/\\");
    std::string fileName = filePath.substr(fileNamePos + 1);
    std::string ext(""), upperExt("");
    size_t extPos = fileName.find_last_of(".");
    if (extPos != std::string::npos)
    {
        ext = fileName.substr(extPos + 1);
        upperExt = ext;
        std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
    }
    // Check if file is a IMG or SET file
    if (upperExt == "IMG" || upperExt == "SET")
    {
        size_t fileSize;
        std::ifstream ifstream;
        ifstream.open(filePath, std::ios_base::in | std::ios_base::binary);
        if (ifstream.is_open())
        {
            ifstream.seekg(0, std::ios_base::end);
            fileSize = ifstream.tellg();
            ifstream.seekg(0, std::ios_base::beg);
            // if a SET file
            if (upperExt == "SET")
            {
                width = 64;
                height = fileSize/64;
                flags = 0xFFFF;
                if (fileSize > 64000)
                {
                    delete[] imageData;
                    imageData = new uchar[fileSize];
                }
                ifstream.read(reinterpret_cast<char*>(imageData), fileSize);
                Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                palette = "PAL.COL";
                Img.setColorTable(palettesMap[palette]);
                hasTransparency = false;
                return Img;
            }
            // if a IMG file
            else
            {
                // if size = 4096 (texture 64x64)
                if (fileSize == 4096)
                {
                    width = 64;
                    height = 64;
                    flags = 0xFFFF;
                    ifstream.read(reinterpret_cast<char*>(imageData), fileSize);
                    Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                    Img.setColorTable(transparentPalettesMap["PAL.COL"]);
                    hasTransparency = true;
                    palette = "PAL.COL";
                    return Img;
                }
                // if IMG with no header but dimensions to set manually
                for (int i(0); i < 15; i++)
                {
                    if (fileName == noHeaderIMG[i].fileName)
                    {
                        width = noHeaderIMG[i].width;
                        height = noHeaderIMG[i].height;
                        flags = 0xFFFF;
                        ifstream.read(reinterpret_cast<char*>(imageData), fileSize);
                        Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                        for (int i(0); i<947; i++)
                        {
                            if (nativeIMGsList[i].fileName == fileName)
                            {
                                palette = nativeIMGsList[i].paletteName;
                                Img.setColorTable(transparentPalettesMap[palette]);
                                hasTransparency = true;
                                return Img;
                            }
                        }
                    }
                }
                // All other IMG should have a 12 bytes header
                ifstream.read(reinterpret_cast<char*>(&offsetX), 2);
                ifstream.read(reinterpret_cast<char*>(&offsetY), 2);
                ifstream.read(reinterpret_cast<char*>(&width), 2);
                ifstream.read(reinterpret_cast<char*>(&height), 2);
                ifstream.read(reinterpret_cast<char*>(&flags), 2);
                ifstream.read(reinterpret_cast<char*>(&dataSize), 2);
                // IMG compressed in 08 has the decompressed size stored after the header
                if ((flags & 0x00FF) == 0x0008)
                {
                    ifstream.read(reinterpret_cast<char*>(&decompressedSize), 2);
                }
                // Set the buffers bigger if needed
                if (width*height > 64000 || dataSize > 64000)
                {
                    delete[] imageData;
                    imageData = new uchar[width*height];
                    delete[] compressedImageData;
                    compressedImageData = new uchar[dataSize];
                }
                // IMG not compressed
                if ((flags & 0x00FF) == 0x0000)
                {
                    ifstream.read(reinterpret_cast<char*>(imageData), dataSize);
                    Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                }
                // IMG compressed in 04
                else if ((flags & 0x00FF) == 0x0004)
                {
                    ifstream.read(reinterpret_cast<char*>(compressedImageData), dataSize);
                    Compression::image04Decompression(compressedImageData, imageData, dataSize);
                    Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                }
                // IMG compressed in 08
                else if ((flags & 0x00FF) == 0x0008 && decompressedSize != 0)
                {
                    ifstream.read(reinterpret_cast<char*>(compressedImageData), dataSize-2);
                    Compression::image08Decompression(compressedImageData, imageData, dataSize-2, decompressedSize);
                    Img = QImage(imageData, width, height, width, QImage::Format_Indexed8);
                }
                // Unknown IMG compression flag -> invalid IMG file
                else
                {
                    Img = QImage(0, 0, QImage::Format_Indexed8);
                    return Img;
                }
                // In case the IMG has an integrated palette
                if ((flags & 0x0100) == 0x0100)
                {
                    uint8_t R(0), G(0), B(0);
                    palette = "Integrated";
                    palettesMap[palette].clear();
                    transparentPalettesMap[palette].clear();
                    // First color transparent or not
                    ifstream.read(reinterpret_cast<char*>(&R), 1);
                    ifstream.read(reinterpret_cast<char*>(&G), 1);
                    ifstream.read(reinterpret_cast<char*>(&B), 1);
                    if (R != 0)
                    {
                        R = (R << 2) + 3;
                    }
                    if (G != 0)
                    {
                        G = (G << 2) + 3;
                    }
                    if (B != 0)
                    {
                        B = (B << 2) + 3;
                    }
                    // Not transparent if integrated 01
                    if ((flags & 0xFF00) == 0x0100)
                    {
                        palettesMap[palette].append(qRgb(R, G, B));
                        hasTransparency = false;
                    }
                    // Transparent if integrated 09
                    else
                    {
                        transparentPalettesMap[palette].append(qRgba(R, G, B, 1));
                        hasTransparency = true;
                    }
                    // Other 255 color
                    for (size_t i(1); i<256; i++)
                    {
                        ifstream.read(reinterpret_cast<char*>(&R), 1);
                        ifstream.read(reinterpret_cast<char*>(&G), 1);
                        ifstream.read(reinterpret_cast<char*>(&B), 1);
                        // All integrated palette have a max of 0x3F for color value instead of 0xFF (value originally store in 6 bits instead of 8)
                        // Goal is to lighter the palette by converting 6bit color to 8bit with a left bit shift of 2 and adding 3 (if not 0)
                        if (R != 0)
                        {
                            R = (R << 2) + 3;
                        }
                        if (G != 0)
                        {
                            G = (G << 2) + 3;
                        }
                        if (B != 0)
                        {
                            B = (B << 2) + 3;
                        }
                        if ((flags & 0xFF00) == 0x0100)
                        {
                            palettesMap[palette].append(qRgb(R, G, B));
                        }
                        else
                        {
                            transparentPalettesMap[palette].append(qRgb(R, G, B));
                        }
                    }
                    // Not transparent
                    if ((flags & 0xFF00) == 0x0100)
                    {
                        Img.setColorTable(palettesMap[palette]);
                    }
                    // Transparent
                    else
                    {
                        Img.setColorTable(transparentPalettesMap[palette]);
                    }
                    return Img;
                }
                // Choose a color table if IMG has none integrated and is a native Arena IMG
                else
                {
                    for (int i(0); i<947; i++)
                    {
                        if (nativeIMGsList[i].fileName == fileName)
                        {
                            palette = nativeIMGsList[i].paletteName;
                            // If native palette is integrated but actual file palette flag is set to not integrated -> build image with PAL.COL
                            // can be found after getting the QImage by getting flag not 01 or 09 but getting palette integrated
                            if (palette == "Integrated")
                            {
                                Img.setColorTable(palettesMap["PAL.COL"]);
                                hasTransparency = false;
                                return Img;
                            }
                            // Setting up palette with transparency
                            Img.setColorTable(transparentPalettesMap[palette]);
                            hasTransparency = true;
                            return Img;
                        }
                    }
                }
                // Finally set color table to PAL.COL with no transparency if IMG is not a native Arena IMG but format has been validated
                // by the process (returned QImage size != 0) with no integrated palette
                palette = "PAL.COL";
                Img.setColorTable(palettesMap[palette]);
                hasTransparency = false;
                return Img;
            }
        }
        // File could not be opened
        else
        {
            Img = QImage(0, 0, QImage::Format_Indexed8);
            return Img;
        }
    }
    // Not a IMG or SET file
    else
    {
        Img = QImage(0, 0, QImage::Format_Indexed8);
        return Img;
    }
}

// Build animation from filepath (if different of "") or index
std::vector<Image::frameData> Image::buildAnimation(int index, const std::string filePath)
{
    bool buildFromIndex(false), hasNoHeader(false);
    size_t byteProcessed(0);
    size_t fileSize;
    std::string fileName, paletteName;
    uint16_t width, height;
    if (filePath == "")
    {
        buildFromIndex = true;
    }
    std::ifstream *inputFileStream;
    if (buildFromIndex)
    {
        inputFileStream = BSAFile::getInstance()->getStream();
        inputFileStream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
        fileSize = BSAFile::getInstance()->getFileSize(index);
        fileName = BSAFile::getInstance()->getFileName(index);
    }
    else
    {
        inputFileStream = new std::ifstream;
        inputFileStream->open(filePath);
        // If file could not be opened, return empty animation
        if (!inputFileStream->is_open())
        {
            animationData.clear();
            return animationData;
        }
        inputFileStream->seekg(0, std::ios_base::end);
        fileSize = inputFileStream->tellg();
        inputFileStream->seekg(0, std::ios_base::beg);
        int fileNamePos = filePath.find_last_of("/\\");
        fileName = filePath.substr(fileNamePos + 1);
    }
    for (int i(0); i<6; i++)
    {
        if (fileName == noHeaderCIF[i].fileName)
        {
            hasNoHeader = true;
            width = noHeaderCIF[i].width;
            height = noHeaderCIF[i].height;
            paletteName = "PAL.COL";
        }
    }
    if (hasNoHeader)
    {
        animationData.clear();
        while (byteProcessed < fileSize)
        {
            animationData.resize(animationData.size() + 1);
            frameData &frame = animationData[animationData.size() - 1];
            uchar *uncompressedFrameData = new uchar[width*height];
            inputFileStream->read(reinterpret_cast<char*>(uncompressedFrameData), width*height);
            frame.hasTransparency = false;
            frame.imagePaletteName = paletteName;
            frame.imagePalette = palettesMap[frame.imagePaletteName];
            QImage img = QImage(uncompressedFrameData, width, height, width, QImage::Format_Indexed8);
            frame.imageQImage = img;
            frame.imageQImage.setColorTable(frame.imagePalette);
            byteProcessed += width*height;
            delete[] uncompressedFrameData;
        }
    }
    else
    {
        animationData.clear();
        while (byteProcessed < fileSize)
        {
            animationData.resize(animationData.size() + 1);
            frameData &frame = animationData[animationData.size() - 1];
            uint32_t offsets;
            inputFileStream->read(reinterpret_cast<char*>(&offsets), 4);
            inputFileStream->read(reinterpret_cast<char*>(&width), 2);
            inputFileStream->read(reinterpret_cast<char*>(&height), 2);
            uint8_t compressionFlag, paletteFlag;
            inputFileStream->read(reinterpret_cast<char*>(&compressionFlag), 1);
            inputFileStream->read(reinterpret_cast<char*>(&paletteFlag), 1);
            uint16_t frameDataSize;
            inputFileStream->read(reinterpret_cast<char*>(&frameDataSize), 2);
            uint16_t uncompressedFrameDataSize = 0x00FF;
            if (compressionFlag == 0x08)
            {
                inputFileStream->read(reinterpret_cast<char*>(&uncompressedFrameDataSize), 2);
            }
            uchar *compressedFrameData = new uchar[frameDataSize];
            uchar *uncompressedFrameData = new uchar[width*height];
            // IMG not compressed
            if (compressionFlag == 0x00)
            {
                inputFileStream->read(reinterpret_cast<char*>(uncompressedFrameData), width*height);
                paletteName = "CHARSHT.COL";
                frame.hasTransparency = false;
                frame.imagePaletteName = paletteName;
                frame.imagePalette = transparentPalettesMap[frame.imagePaletteName];
                QImage img = QImage(uncompressedFrameData, width, height, width, QImage::Format_Indexed8);
                frame.imageQImage = img;
                frame.imageQImage.setColorTable(frame.imagePalette);
                byteProcessed += width*height;
            }
            // IMG compressed in 04
            else if (compressionFlag == 0x04)
            {
                inputFileStream->read(reinterpret_cast<char*>(compressedFrameData), frameDataSize);
                Compression::image04Decompression(compressedFrameData, uncompressedFrameData, frameDataSize);
                paletteName = "CHARSHT.COL";
                frame.hasTransparency = false;
                frame.imagePaletteName = paletteName;
                frame.imagePalette = transparentPalettesMap[frame.imagePaletteName];
                QImage img = QImage(uncompressedFrameData, width, height, width, QImage::Format_Indexed8);
                frame.imageQImage = img;
                frame.imageQImage.setColorTable(frame.imagePalette);
                byteProcessed += 12 + frameDataSize;
            }
            // IMG compressed in 08
            else if (compressionFlag == 0x08 && uncompressedFrameDataSize != 0)
            {
                inputFileStream->read(reinterpret_cast<char*>(compressedFrameData), frameDataSize - 2);
                Compression::image08Decompression(compressedFrameData, uncompressedFrameData, frameDataSize-2, uncompressedFrameDataSize);
                paletteName = "CHARSHT.COL";
                frame.hasTransparency = false;
                frame.imagePaletteName = paletteName;
                frame.imagePalette = transparentPalettesMap[frame.imagePaletteName];
                QImage img = QImage(uncompressedFrameData, width, height, width, QImage::Format_Indexed8);
                frame.imageQImage = img;
                frame.imageQImage.setColorTable(frame.imagePalette);
                byteProcessed += 12 + frameDataSize;
            }
            // Unknown IMG compression flag -> invalid IMG file
            else
            {
                inputFileStream->read(reinterpret_cast<char*>(compressedFrameData), frameDataSize);
                paletteName = "PAL.COL";
                frame.hasTransparency = false;
                frame.imagePaletteName = paletteName;
                frame.imagePalette = palettesMap[frame.imagePaletteName];
                QImage img = QImage(0, 0, QImage::Format_Indexed8);
                frame.imageQImage = img;
                frame.imageQImage.setColorTable(frame.imagePalette);
                byteProcessed += 12 + frameDataSize;
            }
            delete[] compressedFrameData;
            delete[] uncompressedFrameData;
        }
    }
    if (!buildFromIndex)
    {
        delete inputFileStream;
    }
    return animationData;
}

// Extract and decompress an IMG/SET file
// return: 0 all good, 1 error while writting file, 2 unknown flags IMG extracted as raw
int Image::extractDecompressImage(int index, const std::string &filePath)
{
    std::string extension = BSAFile::getInstance()->getFileExtension(index);
    bool hasNoHeader(false);
    for (int i(0); i < 15; i++)
    {
        std::string fileName = BSAFile::getInstance()->getFileName(index);
        if (fileName == noHeaderIMG[i].fileName)
        {
            hasNoHeader = true;
        }
    }

    std::ifstream *stream = BSAFile::getInstance()->getStream();
    // If file is a SET file or IMG with no header or IMG of size 64x64 (4096)
    if (extension == "SET" || (extension == "IMG" && BSAFile::getInstance()->getFileSize(index) == 4096) || (extension == "IMG" && hasNoHeader == true))
    {
        size_t fileSize = BSAFile::getInstance()->getFileSize(index);
        char *fileData = new char[fileSize];
        stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
        stream->read(fileData, fileSize);
        std::ofstream OfStream;
        OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
        if (OfStream.is_open())
        {
            OfStream.write(fileData, fileSize);
            delete[] fileData;
            return 0;
        }
        else
        {
            delete[] fileData;
            return 1;
        }
    }
    // If file is a IMG file with header
    else
    {
        // All other IMG
        // Get value from the 12 bytes header
        uint32_t offsets;
        uint16_t sizeX, sizeY, dataSize, decompressedSize;
        uint8_t compressionFlag, paletteFlag;
        stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
        stream->read(reinterpret_cast<char*>(&offsets), 4);
        stream->read(reinterpret_cast<char*>(&sizeX), 2);
        stream->read(reinterpret_cast<char*>(&sizeY), 2);
        stream->read(reinterpret_cast<char*>(&compressionFlag), 1);
        stream->read(reinterpret_cast<char*>(&paletteFlag), 1);
        stream->read(reinterpret_cast<char*>(&dataSize), 2);
        // IMG compressed in 08 has the decompressed size stored after the header
        if (compressionFlag == 0x08)
        {
            stream->read(reinterpret_cast<char*>(&decompressedSize), 2);
        }
        // IMG not compressed
        if (compressionFlag == 0x00)
        {
            char *fileData = new char[dataSize];
            stream->read(fileData, dataSize);
            std::ofstream OfStream;
            OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
            if (OfStream.is_open())
            {
                OfStream.write(reinterpret_cast<char*>(&offsets), 4);
                OfStream.write(reinterpret_cast<char*>(&sizeX), 2);
                OfStream.write(reinterpret_cast<char*>(&sizeY), 2);
                OfStream.write(reinterpret_cast<char*>(&compressionFlag), 1);
                OfStream.write(reinterpret_cast<char*>(&paletteFlag), 1);
                OfStream.write(reinterpret_cast<char*>(&dataSize), 2);
                OfStream.write(fileData, dataSize);
                delete[] fileData;
                if (paletteFlag == 0x01 || paletteFlag == 0x09)
                {
                    char *paletteData = new char[768];
                    stream->read(paletteData, 768);
                    OfStream.write(paletteData, 768);
                    delete[] paletteData;
                }
                return 0;
            }
            else
            {
                delete[] fileData;
                return 1;
            }
        }
        // IMG compressed in 04
        else if (compressionFlag == 0x04)
        {
            unsigned char *fileData = new unsigned char[dataSize];
            unsigned char *decompressedData = new unsigned char[sizeX*sizeY];
            stream->read(reinterpret_cast<char*>(fileData), dataSize);
            Compression::image04Decompression(fileData, decompressedData, dataSize);
            std::ofstream OfStream;
            OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
            if (OfStream.is_open())
            {
                OfStream.write(reinterpret_cast<char*>(&offsets), 4);
                OfStream.write(reinterpret_cast<char*>(&sizeX), 2);
                OfStream.write(reinterpret_cast<char*>(&sizeY), 2);
                uint8_t newCompressionFlag = 0x00;
                OfStream.write(reinterpret_cast<char*>(&newCompressionFlag), 1);
                OfStream.write(reinterpret_cast<char*>(&paletteFlag), 1);
                uint16_t newDataSize = sizeX*sizeY;
                OfStream.write(reinterpret_cast<char*>(&newDataSize), 2);
                OfStream.write(reinterpret_cast<char*>(decompressedData), newDataSize);
                delete[] fileData;
                delete [] decompressedData;
                if (paletteFlag == 0x01 || paletteFlag == 0x09)
                {
                    char *paletteData = new char[768];
                    stream->read(paletteData, 768);
                    OfStream.write(paletteData, 768);
                    delete[] paletteData;
                }
                return 0;
            }
            else
            {
                delete[] fileData;
                delete[] decompressedData;
                return 1;
            }
        }
        // IMG compressed in 08
        else if (compressionFlag == 0x08)
        {
            unsigned char *fileData = new unsigned char[dataSize];
            unsigned char *decompressedData = new unsigned char[sizeX*sizeY];
            stream->read(reinterpret_cast<char*>(fileData), dataSize-2);
            Compression::image08Decompression(fileData, decompressedData, dataSize-2, decompressedSize);
            std::ofstream OfStream;
            OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
            if (OfStream.is_open())
            {
                OfStream.write(reinterpret_cast<char*>(&offsets), 4);
                OfStream.write(reinterpret_cast<char*>(&sizeX), 2);
                OfStream.write(reinterpret_cast<char*>(&sizeY), 2);
                uint8_t newCompressionFlag = 0x00;
                OfStream.write(reinterpret_cast<char*>(&newCompressionFlag), 1);
                OfStream.write(reinterpret_cast<char*>(&paletteFlag), 1);
                OfStream.write(reinterpret_cast<char*>(&decompressedSize), 2);
                OfStream.write(reinterpret_cast<char*>(decompressedData), decompressedSize);
                delete[] fileData;
                delete[] decompressedData;
                if (paletteFlag == 0x01 || paletteFlag == 0x09)
                {
                    char *paletteData = new char[768];
                    stream->read(paletteData, 768);
                    OfStream.write(paletteData, 768);
                    delete[] paletteData;
                }
                return 0;
            }
            else
            {
                delete[] fileData;
                delete[] decompressedData;
                return 1;
            }
        }
        // Unknown IMG header flags
        else
        {
            size_t fileSize = BSAFile::getInstance()->getFileSize(index);
            char *fileData = new char[fileSize];
            stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
            stream->read(fileData, fileSize);
            std::ofstream OfStream;
            OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
            if (OfStream.is_open())
            {
                OfStream.write(fileData, fileSize);
                delete[] fileData;
                return 2;
            }
            else
            {
                delete[] fileData;
                return 1;
            }
        }
    }
}

// Extract, decompress and convert an IMG/SET file to png
// return 0: ok, 1: error while extracting, 2: unknown header IMG not extracted
int Image::extractDecompressConvertImage(int index, const std::string &filePath)
{
    std::string ext = BSAFile::getInstance()->getFileExtension(index);
    QImage image = buildQImage(index, ext);
    if (image.size() != QSize(0, 0))
    {
        bool ret = image.save(QString::fromStdString(filePath), "png");
        if (ret == true)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 2;
    }
}

// Decompress external IMG
// return 0: ok, 1: error while opening or writting, 2: IMG with no header or not compressed not modified ,3: unknown header IMG not modified, 4: not an IMG file
int Image::decompressExternalIMG(const std::string &fileName, const std::string &filePath)
{
    std::string ext(""), upperExt("");
    size_t extPos = fileName.find_last_of(".");
    if (extPos != std::string::npos)
    {
        ext = fileName.substr(extPos + 1);
        upperExt = ext;
        std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
    }
    if (upperExt == "IMG")
    {
        std::ifstream ifstream;
        ifstream.open(filePath, std::ios_base::in | std::ios_base::binary);
        if (ifstream.is_open())
        {
            ifstream.seekg(0, std::ios_base::end);
            size_t fileSize = ifstream.tellg();
            ifstream.seekg(0, std::ios_base::beg);
            bool hasNoHeader(false);
            for (int i(0); i < 15; i++)
            {
                if (fileName == noHeaderIMG[i].fileName)
                {
                    hasNoHeader = true;
                }
            }
            if (hasNoHeader == true || fileSize == 4096)
            {
                return 2;
            }
            else
            {
                // Get value from the 12 bytes header
                uint32_t offsets;
                uint16_t sizeX, sizeY, dataSize, decompressedSize;
                uint8_t compressionFlag, paletteFlag;
                ifstream.read(reinterpret_cast<char*>(&offsets), 4);
                ifstream.read(reinterpret_cast<char*>(&sizeX), 2);
                ifstream.read(reinterpret_cast<char*>(&sizeY), 2);
                ifstream.read(reinterpret_cast<char*>(&compressionFlag), 1);
                ifstream.read(reinterpret_cast<char*>(&paletteFlag), 1);
                ifstream.read(reinterpret_cast<char*>(&dataSize), 2);
                // IMG compressed in 08 has the decompressed size stored after the header
                if (compressionFlag == 0x08)
                {
                    ifstream.read(reinterpret_cast<char*>(&decompressedSize), 2);
                }
                // IMG not compressed
                if (compressionFlag == 0x00)
                {
                    return 2;
                }
                // IMG compressed in 04
                else if (compressionFlag == 0x04)
                {
                    unsigned char *fileData = new unsigned char[dataSize];
                    unsigned char *decompressedData = new unsigned char[sizeX*sizeY];
                    char *paletteData = new char[768];
                    ifstream.read(reinterpret_cast<char*>(fileData), dataSize);
                    Compression::image04Decompression(fileData, decompressedData, dataSize);
                    if (paletteFlag == 0x01 || paletteFlag == 0x09)
                    {
                        ifstream.read(paletteData, 768);
                    }
                    ifstream.close();
                    std::ofstream OfStream;
                    OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
                    if (OfStream.is_open())
                    {
                        OfStream.write(reinterpret_cast<char*>(&offsets), 4);
                        OfStream.write(reinterpret_cast<char*>(&sizeX), 2);
                        OfStream.write(reinterpret_cast<char*>(&sizeY), 2);
                        uint8_t newCompressionFlag = 0x00;
                        OfStream.write(reinterpret_cast<char*>(&newCompressionFlag), 1);
                        OfStream.write(reinterpret_cast<char*>(&paletteFlag), 1);
                        uint16_t newDataSize = sizeX*sizeY;
                        OfStream.write(reinterpret_cast<char*>(&newDataSize), 2);
                        OfStream.write(reinterpret_cast<char*>(decompressedData), newDataSize);
                        if (paletteFlag == 0x01 || paletteFlag == 0x09)
                        {
                            OfStream.write(paletteData, 768);
                        }
                        delete[] paletteData;
                        delete[] fileData;
                        delete [] decompressedData;
                        return 0;
                    }
                    else
                    {
                        delete[] paletteData;
                        delete[] fileData;
                        delete[] decompressedData;
                        return 1;
                    }
                }
                // IMG compressed in 08
                else if (compressionFlag == 0x08)
                {
                    unsigned char *fileData = new unsigned char[dataSize];
                    unsigned char *decompressedData = new unsigned char[sizeX*sizeY];
                    char *paletteData = new char[768];
                    ifstream.read(reinterpret_cast<char*>(fileData), dataSize-2);
                    Compression::image08Decompression(fileData, decompressedData, dataSize-2, decompressedSize);
                    if (paletteFlag == 0x01 || paletteFlag == 0x09)
                    {
                        ifstream.read(paletteData, 768);
                    }
                    ifstream.close();
                    std::ofstream OfStream;
                    OfStream.open(filePath, std::ios_base::out | std::ios_base::binary);
                    if (OfStream.is_open())
                    {
                        OfStream.write(reinterpret_cast<char*>(&offsets), 4);
                        OfStream.write(reinterpret_cast<char*>(&sizeX), 2);
                        OfStream.write(reinterpret_cast<char*>(&sizeY), 2);
                        uint8_t newCompressionFlag = 0x00;
                        OfStream.write(reinterpret_cast<char*>(&newCompressionFlag), 1);
                        OfStream.write(reinterpret_cast<char*>(&paletteFlag), 1);
                        OfStream.write(reinterpret_cast<char*>(&decompressedSize), 2);
                        OfStream.write(reinterpret_cast<char*>(decompressedData), decompressedSize);
                        if (paletteFlag == 0x01 || paletteFlag == 0x09)
                        {
                            OfStream.write(paletteData, 768);
                        }
                        delete[] paletteData;
                        delete[] fileData;
                        delete [] decompressedData;
                        return 0;
                    }
                    else
                    {
                        delete[] paletteData;
                        delete[] fileData;
                        delete[] decompressedData;
                        return 1;
                    }
                }
                // Unknown IMG header flags
                else
                {
                    return 3;
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 4;
    }
}

// Return image flags
uint16_t Image::getImageFlags()
{
    return flags;
}

// Return image width
uint16_t Image::getImageWidth()
{
    return width;
}

// Return image height
uint16_t Image::getImageHeight()
{
    return height;
}

// Return true if filename is one of the native IMG
bool Image::getIsNativeIMG(const std::string &fileName)
{
    bool isNative(false);
    for (int i(0); i<947; i++)
    {
        if (nativeIMGsList[i].fileName == fileName)
        {
            isNative = true;
        }
    }
    return isNative;
}

// Return true if filename is one of the native SET
bool Image::getIsNativeSET(const std::string &fileName)
{
    bool isNative(false);
    for (int i(0); i<180; i++)
    {
        if (nativeSETsList[i].fileName == fileName)
        {
            isNative = true;
        }
    }
    return isNative;
}

// return width of the SET image if native or 0 if not
int Image::getNativeSETWidth(const std::string &fileName)
{
    for (int i(0); i<180; i++)
    {
        if (nativeSETsList[i].fileName == fileName)
        {
            return nativeSETsList[i].width;
        }
    }
    return 0;
}

// return height of the SET image if native or 0 if not
int Image::getNativeSETHeight(const std::string &fileName)
{
    for (int i(0); i<180; i++)
    {
        if (nativeSETsList[i].fileName == fileName)
        {
            return nativeSETsList[i].height;
        }
    }
    return 0;
}

// Return palette type (texture/noHeader/0/1/8/9) if filename is one of the native IMG, empty string otherwise
std::string Image::getNativeIMGPaletteType(const std::string &fileName)
{
    for (int i(0); i<947; i++)
    {
        if (nativeIMGsList[i].fileName == fileName)
        {
            return nativeIMGsList[i].paletteType;
        }
    }
    return "";
}

// Return name of the palette used to build the last QImage
std::string Image::getPaletteName()
{
    return palette;
}

// Return palette used to build the last QImage
QVector<QRgb> Image::getPalette(const std::string &palette, bool transparency)
{
    if (transparency == false)
    {
        return palettesMap[palette];
    }
    else
    {
        return transparentPalettesMap[palette];
    }
}

// Return transparency value for the last built QImage
bool Image::getTransparency()
{
    return hasTransparency;
}

// Return Arena native palette file list
QStringList Image::getPaletteFileList()
{
    return nativePaletteFileList;
}

// Return noHeaderIMG.width or 0 if image is not in the list
int Image::getNoHeaderIMGWidth(const std::string &fileName)
{
    for (int i(0); i<15; i++)
    {
        if (noHeaderIMG[i].fileName == fileName)
        {
            return noHeaderIMG[i].width;
        }
    }
    return 0;
}

// Return noHeaderIMG.height or 0 if image is not in the list
int Image::getNoHeaderIMGHeight(const std::string &fileName)
{
    for (int i(0); i<15; i++)
    {
        if (noHeaderIMG[i].fileName == fileName)
        {
            return noHeaderIMG[i].height;
        }
    }
    return 0;
}
