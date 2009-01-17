package cn.sda.pinyin;

public class Spell2Chars {

	private Spell2Chars() {
	}

	public static String getSpell(int spellNumber) {
		String curSpells = getInput2Spell(spellNumber);
		if (curSpells == null) {
			int i = 2;
			do {
				if (i >= 10)
					break;
				curSpells = getInput2Spell(spellNumber * 10 + i);
				if (curSpells != null) {
					break;
				}
				i++;
			} while (true);
		}
		return curSpells.toLowerCase();
	}

	public static String getInput2Spell(int spellNumber) {
		String result = null;
		if (spellNumber == 6826)
			result = "NUAN";
		else if (spellNumber == 468)
			result = "GOU HOU";
		else if (spellNumber == 5364)
			result = "KENG LENG";
		else if (spellNumber == 268)
			result = "COU";
		else if (spellNumber == 264)
			result = "ANG";
		else if (spellNumber == 6426)
			result = "MIAN MIAO NIAN NIAO";
		else if (spellNumber == 3664)
			result = "DONG";
		else if (spellNumber == 0x1244a)
			result = "SHUAN";
		else if (spellNumber == 0x12448)
			result = "SHUAI";
		else if (spellNumber == 3464)
			result = "DING";
		else if (spellNumber == 64264)
			result = "NIANG";
		else if (spellNumber == 3264)
			result = "DANG FANG";
		else if (spellNumber == 2436)
			result = "CHEN";
		else if (spellNumber == 8664)
			result = "TONG";
		else if (spellNumber == 2826)
			result = "CUAN";
		else if (spellNumber == 843)
			result = "TIE";
		else if (spellNumber == 648)
			result = "MIU NIU";
		else if (spellNumber == 8464)
			result = "TING";
		else if (spellNumber == 646)
			result = "MIN NIN";
		else if (spellNumber == 643)
			result = "MIE NIE";
		else if (spellNumber == 24664)
			result = "CHONG";
		else if (spellNumber == 8264)
			result = "TANG";
		else if (spellNumber == 2426)
			result = "BIAN BIAO CHAN CHAO";
		else if (spellNumber == 7436)
			result = "SHEN";
		else if (spellNumber == 2424)
			result = "CHAI";
		else if (spellNumber == 248)
			result = "CHU";
		else if (spellNumber == 246)
			result = "BIN";
		else if (spellNumber == 0x171c8)
			result = "XIONG ZHONG";
		else if (spellNumber == 244)
			result = "CHI";
		else if (spellNumber == 243)
			result = "BIE CHE";
		else if (spellNumber == 9486)
			result = "ZHUN ZHUO";
		else if (spellNumber == 242)
			result = "CHA";
		else if (spellNumber == 24264)
			result = "CHANG";
		else if (spellNumber == 9484)
			result = "ZHUI";
		else if (spellNumber == 9482)
			result = "ZHUA";
		else if (spellNumber == 7826)
			result = "QUAN RUAN SUAN";
		else if (spellNumber == 58264)
			result = "KUANG";
		else if (spellNumber == 6364)
			result = "MENG NENG";
		else if (spellNumber == 0x17038)
			result = "XIANG ZHANG";
		else if (spellNumber == 636)
			result = "MEN NEN";
		else if (spellNumber == 98)
			result = "WU XU YU ZU";
		else if (spellNumber == 0xe7828)
			result = "ZHUANG";
		else if (spellNumber == 634)
			result = "MEI NEI";
		else if (spellNumber == 96)
			result = "WO YO";
		else if (spellNumber == 94)
			result = "XI YI ZI";
		else if (spellNumber == 93)
			result = "YE ZE";
		else if (spellNumber == 92)
			result = "WA YA ZA";
		else if (spellNumber == 436)
			result = "GEN HEN";
		else if (spellNumber == 7426)
			result = "PIAN PIAO QIAN QIAO SHAN SHAO";
		else if (spellNumber == 7424)
			result = "SHAI";
		else if (spellNumber == 434)
			result = "GEI HEI";
		else if (spellNumber == 4664)
			result = "GONG HONG";
		else if (spellNumber == 236)
			result = "BEN CEN";
		else if (spellNumber == 234)
			result = "BEI";
		else if (spellNumber == 0x1227c)
			result = "SHENG";
		else if (spellNumber == 826)
			result = "TAN TAO";
		else if (spellNumber == 986)
			result = "XUN YUN ZUN ZUO";
		else if (spellNumber == 824)
			result = "TAI";
		else if (spellNumber == 4264)
			result = "GANG HANG";
		else if (spellNumber == 984)
			result = "ZUI";
		else if (spellNumber == 983)
			result = "XUE YUE";
		else if (spellNumber == 626)
			result = "MAN MAO NAN NAO";
		else if (spellNumber == 88)
			result = "TU";
		else if (spellNumber == 786)
			result = "QUN RUN RUO SUN SUO";
		else if (spellNumber == 624)
			result = "MAI NAI";
		else if (spellNumber == 54664)
			result = "JIONG";
		else if (spellNumber == 784)
			result = "RUI SUI";
		else if (spellNumber == 84)
			result = "TI";
		else if (spellNumber == 783)
			result = "QUE";
		else if (spellNumber == 83)
			result = "TE";
		else if (spellNumber == 82)
			result = "TA";
		else if (spellNumber == 426)
			result = "GAN GAO HAN HAO";
		else if (spellNumber == 424)
			result = "GAI HAI";
		else if (spellNumber == 586)
			result = "JUN KUN KUO LUN LUO";
		else if (spellNumber == 584)
			result = "KUI";
		else if (spellNumber == 583)
			result = "JUE LUE";
		else if (spellNumber == 9664)
			result = "YONG ZONG";
		else if (spellNumber == 582)
			result = "KUA";
		else if (spellNumber == 3826)
			result = "DUAN";
		else if (spellNumber == 226)
			result = "BAN BAO CAN CAO";
		else if (spellNumber == 9468)
			result = "ZHOU";
		else if (spellNumber == 54264)
			result = "JIANG LIANG";
		else if (spellNumber == 224)
			result = "BAI CAI";
		else if (spellNumber == 386)
			result = "DUN DUO";
		else if (spellNumber == 2364)
			result = "BENG CENG";
		else if (spellNumber == 384)
			result = "DUI";
		else if (spellNumber == 9464)
			result = "XING YING";
		else if (spellNumber == 9264)
			result = "WANG YANG ZANG";
		else if (spellNumber == 3426)
			result = "DIAN DIAO";
		else if (spellNumber == 78)
			result = "PU QU RU SU";
		else if (spellNumber == 76)
			result = "PO";
		else if (spellNumber == 74)
			result = "PI QI RI SI";
		else if (spellNumber == 73)
			result = "RE SE";
		else if (spellNumber == 72)
			result = "PA SA";
		else if (spellNumber == 0x3c9c8)
			result = "CHUANG";
		else if (spellNumber == 8826)
			result = "TUAN";
		else if (spellNumber == 7364)
			result = "PENG RENG SENG";
		else if (spellNumber == 968)
			result = "YOU ZOU";
		else if (spellNumber == 24826)
			result = "CHUAN";
		else if (spellNumber == 8426)
			result = "TIAN TIAO";
		else if (spellNumber == 24824)
			result = "CHUAI";
		else if (spellNumber == 768)
			result = "POU ROU SOU";
		else if (spellNumber == 68)
			result = "MU NU OU";
		else if (spellNumber == 5664)
			result = "KONG LONG";
		else if (spellNumber == 66)
			result = "MO";
		else if (spellNumber == 64)
			result = "MI NG NI";
		else if (spellNumber == 0x1726a)
			result = "ZHUAN";
		else if (spellNumber == 63)
			result = "ME NE";
		else if (spellNumber == 62)
			result = "MA NA";
		else if (spellNumber == 568)
			result = "KOU LOU";
		else if (spellNumber == 0x17268)
			result = "ZHUAI";
		else if (spellNumber == 5464)
			result = "JING LING";
		else if (spellNumber == 48264)
			result = "GUANG HUANG";
		else if (spellNumber == 368)
			result = "DOU FOU";
		else if (spellNumber == 5264)
			result = "KANG LANG";
		else if (spellNumber == 58)
			result = "JU KU LU";
		else if (spellNumber == 4826)
			result = "GUAN HUAN";
		else if (spellNumber == 4824)
			result = "GUAI HUAI";
		else if (spellNumber == 54)
			result = "JI LI";
		else if (spellNumber == 53)
			result = "KE LE";
		else if (spellNumber == 3364)
			result = "DENG FENG";
		else if (spellNumber == 52)
			result = "KA LA";
		else if (spellNumber == 9436)
			result = "ZHEN";
		else if (spellNumber == 948)
			result = "XIU ZHU";
		else if (spellNumber == 946)
			result = "XIN YIN";
		else if (spellNumber == 944)
			result = "ZHI";
		else if (spellNumber == 943)
			result = "XIE ZHE";
		else if (spellNumber == 942)
			result = "XIA ZHA";
		else if (spellNumber == 748)
			result = "QIU SHU";
		else if (spellNumber == 48)
			result = "GU HU";
		else if (spellNumber == 746)
			result = "PIN QIN";
		else if (spellNumber == 9826)
			result = "XUAN YUAN ZUAN";
		else if (spellNumber == 744)
			result = "SHI";
		else if (spellNumber == 743)
			result = "PIE QIE SHE";
		else if (spellNumber == 742)
			result = "QIA SHA";
		else if (spellNumber == 43)
			result = "GE HE";
		else if (spellNumber == 548)
			result = "JIU LIU";
		else if (spellNumber == 42)
			result = "GA HA";
		else if (spellNumber == 8364)
			result = "TENG";
		else if (spellNumber == 546)
			result = "JIN LIN";
		else if (spellNumber == 543)
			result = "JIE LIE";
		else if (spellNumber == 542)
			result = "JIA LIA";
		else if (spellNumber == 348)
			result = "DIU";
		else if (spellNumber == 2486)
			result = "CHUN CHUO";
		else if (spellNumber == 9426)
			result = "XIAN XIAO ZHAN ZHAO";
		else if (spellNumber == 343)
			result = "DIE";
		else if (spellNumber == 2484)
			result = "CHUI";
		else if (spellNumber == 9424)
			result = "ZHAI";
		else if (spellNumber == 342)
			result = "DIA";
		else if (spellNumber == 24364)
			result = "CHENG";
		else if (spellNumber == 6664)
			result = "NONG";
		else if (spellNumber == 936)
			result = "WEN ZEN";
		else if (spellNumber == 934)
			result = "WEI ZEI";
		else if (spellNumber == 6464)
			result = "MING NING";
		else if (spellNumber == 0x1709c)
			result = "ZHENG";
		else if (spellNumber == 0xb6ae8)
			result = "SHUANG";
		else if (spellNumber == 38)
			result = "DU FU";
		else if (spellNumber == 736)
			result = "PEN REN SEN";
		else if (spellNumber == 37)
			result = "ER";
		else if (spellNumber == 36)
			result = "EN FO";
		else if (spellNumber == 734)
			result = "PEI";
		else if (spellNumber == 34)
			result = "DI EI";
		else if (spellNumber == 6264)
			result = "MANG NANG";
		else if (spellNumber == 33)
			result = "DE";
		else if (spellNumber == 32)
			result = "DA FA";
		else if (spellNumber == 536)
			result = "KEN";
		else if (spellNumber == 534)
			result = "LEI";
		else if (spellNumber == 0x123a8)
			result = "QIONG";
		else if (spellNumber == 336)
			result = "FEN";
		else if (spellNumber == 7486)
			result = "SHUN SHUO";
		else if (spellNumber == 334)
			result = "FEI";
		else if (spellNumber == 7484)
			result = "SHUI";
		else if (spellNumber == 7482)
			result = "SHUA";
		else if (spellNumber == 5826)
			result = "JUAN KUAN LUAN";
		else if (spellNumber == 926)
			result = "WAN YAN YAO ZAN ZAO";
		else if (spellNumber == 5824)
			result = "KUAI";
		else if (spellNumber == 924)
			result = "WAI ZAI";
		else if (spellNumber == 4364)
			result = "GENG HENG";
		else if (spellNumber == 0x12218)
			result = "QIANG SHANG";
		else if (spellNumber == 28)
			result = "BU CU";
		else if (spellNumber == 726)
			result = "PAN PAO RAN RAO SAN SAO";
		else if (spellNumber == 26)
			result = "AN AO BO";
		else if (spellNumber == 886)
			result = "TUN TUO";
		else if (spellNumber == 724)
			result = "PAI SAI";
		else if (spellNumber == 24)
			result = "AI BI CI";
		else if (spellNumber == 884)
			result = "TUI";
		else if (spellNumber == 23)
			result = "CE";
		else if (spellNumber == 22)
			result = "BA CA";
		else if (spellNumber == 5426)
			result = "JIAN JIAO LIAN LIAO";
		else if (spellNumber == 526)
			result = "KAN KAO LAN LAO";
		else if (spellNumber == 524)
			result = "KAI LAI";
		else if (spellNumber == 686)
			result = "NUO";
		else if (spellNumber == 2664)
			result = "CONG";
		else if (spellNumber == 683)
			result = "NUE";
		else if (spellNumber == 2468)
			result = "CHOU";
		else if (spellNumber == 326)
			result = "DAN DAO FAN";
		else if (spellNumber == 486)
			result = "GUN GUO HUN HUO";
		else if (spellNumber == 324)
			result = "DAI";
		else if (spellNumber == 2464)
			result = "BING";
		else if (spellNumber == 484)
			result = "GUI HUI";
		else if (spellNumber == 6)
			result = "M O";
		else if (spellNumber == 482)
			result = "GUA HUA";
		else if (spellNumber == 3)
			result = "E";
		else if (spellNumber == 2)
			result = "A";
		else if (spellNumber == 286)
			result = "CUN CUO";
		else if (spellNumber == 2264)
			result = "BANG CANG";
		else if (spellNumber == 284)
			result = "CUI";
		else if (spellNumber == 9364)
			result = "WENG ZENG";
		else if (spellNumber == 7664)
			result = "RONG SONG";
		else if (spellNumber == 7468)
			result = "SHOU";
		else if (spellNumber == 7464)
			result = "PING QING";
		else if (spellNumber == 7264)
			result = "PANG RANG SANG";
		else if (spellNumber == 868)
			result = "TOU";
		else if (spellNumber == 668)
			result = "MOU NOU";
		return result;
	}

	public static char[] getSpell2Chars(StringBuffer spell) {
		char result[] = null;
		if (spell == null || spell.length() <= 0)
			return null;
		int length = spell.length();
		char temp[] = new char[length];
		spell.getChars(0, length, temp, 0);
		if (length == 1) {
			if (temp[0] == 'a')
				result = CharsLibrary_1.A;
			else if (temp[0] == 'e')
				result = CharsLibrary_1.E;
			else if (temp[0] == 'm')
				result = CharsLibrary_2.M;
			else if (temp[0] == 'o')
				result = CharsLibrary_2.O;
		} else if (length == 2) {
			if (temp[0] == 'a' && temp[1] == 'i')
				result = CharsLibrary_1.AI;
			else if (temp[0] == 'a' && temp[1] == 'n')
				result = CharsLibrary_1.AN;
			else if (temp[0] == 'a' && temp[1] == 'o')
				result = CharsLibrary_1.AO;
			else if (temp[0] == 'b' && temp[1] == 'a')
				result = CharsLibrary_1.BA;
			else if (temp[0] == 'b' && temp[1] == 'i')
				result = CharsLibrary_1.BI;
			else if (temp[0] == 'b' && temp[1] == 'o')
				result = CharsLibrary_1.BO;
			else if (temp[0] == 'b' && temp[1] == 'u')
				result = CharsLibrary_1.BU;
			else if (temp[0] == 'c' && temp[1] == 'a')
				result = CharsLibrary_1.CA;
			else if (temp[0] == 'c' && temp[1] == 'e')
				result = CharsLibrary_1.CE;
			else if (temp[0] == 'c' && temp[1] == 'i')
				result = CharsLibrary_1.CI;
			else if (temp[0] == 'c' && temp[1] == 'u')
				result = CharsLibrary_1.CU;
			else if (temp[0] == 'd' && temp[1] == 'a')
				result = CharsLibrary_1.DA;
			else if (temp[0] == 'd' && temp[1] == 'e')
				result = CharsLibrary_1.DE;
			else if (temp[0] == 'd' && temp[1] == 'i')
				result = CharsLibrary_1.DI;
			else if (temp[0] == 'd' && temp[1] == 'u')
				result = CharsLibrary_1.DU;
			else if (temp[0] == 'e' && temp[1] == 'i')
				result = CharsLibrary_1.EI;
			else if (temp[0] == 'e' && temp[1] == 'n')
				result = CharsLibrary_1.EN;
			else if (temp[0] == 'e' && temp[1] == 'r')
				result = CharsLibrary_1.ER;
			else if (temp[0] == 'f' && temp[1] == 'a')
				result = CharsLibrary_1.FA;
			else if (temp[0] == 'f' && temp[1] == 'o')
				result = CharsLibrary_1.FO;
			else if (temp[0] == 'f' && temp[1] == 'u')
				result = CharsLibrary_1.FU;
			else if (temp[0] == 'g' && temp[1] == 'a')
				result = CharsLibrary_1.GA;
			else if (temp[0] == 'g' && temp[1] == 'e')
				result = CharsLibrary_1.GE;
			else if (temp[0] == 'g' && temp[1] == 'u')
				result = CharsLibrary_1.GU;
			else if (temp[0] == 'h' && temp[1] == 'a')
				result = CharsLibrary_1.HA;
			else if (temp[0] == 'h' && temp[1] == 'e')
				result = CharsLibrary_1.HE;
			else if (temp[0] == 'h' && temp[1] == 'u')
				result = CharsLibrary_1.HU;
			else if (temp[0] == 'j' && temp[1] == 'i')
				result = CharsLibrary_1.JI;
			else if (temp[0] == 'j' && temp[1] == 'u')
				result = CharsLibrary_1.JU;
			else if (temp[0] == 'k' && temp[1] == 'a')
				result = CharsLibrary_1.KA;
			else if (temp[0] == 'k' && temp[1] == 'e')
				result = CharsLibrary_1.KE;
			else if (temp[0] == 'k' && temp[1] == 'u')
				result = CharsLibrary_1.KU;
			else if (temp[0] == 'l' && temp[1] == 'a')
				result = CharsLibrary_2.LA;
			else if (temp[0] == 'l' && temp[1] == 'e')
				result = CharsLibrary_2.LE;
			else if (temp[0] == 'l' && temp[1] == 'i')
				result = CharsLibrary_2.LI;
			else if (temp[0] == 'l' && temp[1] == 'u')
				result = CharsLibrary_2.LU;
			else if (temp[0] == 'm' && temp[1] == 'a')
				result = CharsLibrary_2.MA;
			else if (temp[0] == 'm' && temp[1] == 'e')
				result = CharsLibrary_2.ME;
			else if (temp[0] == 'm' && temp[1] == 'i')
				result = CharsLibrary_2.MI;
			else if (temp[0] == 'm' && temp[1] == 'o')
				result = CharsLibrary_2.MO;
			else if (temp[0] == 'm' && temp[1] == 'u')
				result = CharsLibrary_2.MU;
			else if (temp[0] == 'n' && temp[1] == 'a')
				result = CharsLibrary_2.NA;
			else if (temp[0] == 'n' && temp[1] == 'e')
				result = CharsLibrary_2.NE;
			else if (temp[0] == 'n' && temp[1] == 'g')
				result = CharsLibrary_2.NG;
			else if (temp[0] == 'n' && temp[1] == 'i')
				result = CharsLibrary_2.NI;
			else if (temp[0] == 'n' && temp[1] == 'u')
				result = CharsLibrary_2.NU;
			else if (temp[0] == 'n' && temp[1] == 'v')
				result = CharsLibrary_2.NV;
			else if (temp[0] == 'o' && temp[1] == 'u')
				result = CharsLibrary_2.OU;
			else if (temp[0] == 'p' && temp[1] == 'a')
				result = CharsLibrary_2.PA;
			else if (temp[0] == 'p' && temp[1] == 'i')
				result = CharsLibrary_2.PI;
			else if (temp[0] == 'p' && temp[1] == 'o')
				result = CharsLibrary_2.PO;
			else if (temp[0] == 'p' && temp[1] == 'u')
				result = CharsLibrary_2.PU;
			else if (temp[0] == 'q' && temp[1] == 'i')
				result = CharsLibrary_2.QI;
			else if (temp[0] == 'q' && temp[1] == 'u')
				result = CharsLibrary_2.QU;
			else if (temp[0] == 'r' && temp[1] == 'e')
				result = CharsLibrary_2.RE;
			else if (temp[0] == 'r' && temp[1] == 'i')
				result = CharsLibrary_2.RI;
			else if (temp[0] == 'r' && temp[1] == 'u')
				result = CharsLibrary_2.RU;
			else if (temp[0] == 's' && temp[1] == 'a')
				result = CharsLibrary_2.SA;
			else if (temp[0] == 's' && temp[1] == 'e')
				result = CharsLibrary_2.SE;
			else if (temp[0] == 's' && temp[1] == 'i')
				result = CharsLibrary_3.SI;
			else if (temp[0] == 's' && temp[1] == 'u')
				result = CharsLibrary_3.SU;
			else if (temp[0] == 't' && temp[1] == 'a')
				result = CharsLibrary_3.TA;
			else if (temp[0] == 't' && temp[1] == 'e')
				result = CharsLibrary_3.TE;
			else if (temp[0] == 't' && temp[1] == 'i')
				result = CharsLibrary_3.TI;
			else if (temp[0] == 't' && temp[1] == 'u')
				result = CharsLibrary_3.TU;
			else if (temp[0] == 'w' && temp[1] == 'a')
				result = CharsLibrary_3.WA;
			else if (temp[0] == 'w' && temp[1] == 'o')
				result = CharsLibrary_3.WO;
			else if (temp[0] == 'w' && temp[1] == 'u')
				result = CharsLibrary_3.WU;
			else if (temp[0] == 'x' && temp[1] == 'i')
				result = CharsLibrary_3.XI;
			else if (temp[0] == 'x' && temp[1] == 'u')
				result = CharsLibrary_3.XU;
			else if (temp[0] == 'y' && temp[1] == 'a')
				result = CharsLibrary_3.YA;
			else if (temp[0] == 'y' && temp[1] == 'e')
				result = CharsLibrary_3.YE;
			else if (temp[0] == 'y' && temp[1] == 'i')
				result = CharsLibrary_3.YI;
			else if (temp[0] == 'y' && temp[1] == 'o')
				result = CharsLibrary_3.YO;
			else if (temp[0] == 'y' && temp[1] == 'u')
				result = CharsLibrary_3.YU;
			else if (temp[0] == 'z' && temp[1] == 'a')
				result = CharsLibrary_3.ZA;
			else if (temp[0] == 'z' && temp[1] == 'e')
				result = CharsLibrary_3.ZE;
			else if (temp[0] == 'z' && temp[1] == 'i')
				result = CharsLibrary_3.ZI;
			else if (temp[0] == 'z' && temp[1] == 'u')
				result = CharsLibrary_3.ZU;
		} else if (length == 3) {
			if (temp[0] == 'a' && temp[1] == 'n' && temp[2] == 'g')
				result = CharsLibrary_1.ANG;
			else if (temp[0] == 'b' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_1.BAI;
			else if (temp[0] == 'b' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.BAN;
			else if (temp[0] == 'b' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_1.BAO;
			else if (temp[0] == 'b' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_1.BEI;
			else if (temp[0] == 'b' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_1.BEN;
			else if (temp[0] == 'b' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_1.BIE;
			else if (temp[0] == 'b' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_1.BIN;
			else if (temp[0] == 'c' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_1.CAI;
			else if (temp[0] == 'c' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.CAN;
			else if (temp[0] == 'c' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_1.CAO;
			else if (temp[0] == 'c' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_1.CEN;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'a')
				result = CharsLibrary_1.CHA;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'e')
				result = CharsLibrary_1.CHE;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'i')
				result = CharsLibrary_1.CHI;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u')
				result = CharsLibrary_1.CHU;
			else if (temp[0] == 'c' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_1.COU;
			else if (temp[0] == 'c' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_1.CUI;
			else if (temp[0] == 'c' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_1.CUN;
			else if (temp[0] == 'c' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_1.CUO;
			else if (temp[0] == 'd' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_1.DAI;
			else if (temp[0] == 'd' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.DAN;
			else if (temp[0] == 'd' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_1.DAO;
			else if (temp[0] == 'd' && temp[1] == 'i' && temp[2] == 'a')
				result = CharsLibrary_1.DIA;
			else if (temp[0] == 'd' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_1.DIE;
			else if (temp[0] == 'd' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_1.DIU;
			else if (temp[0] == 'd' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_1.DOU;
			else if (temp[0] == 'd' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_1.DUI;
			else if (temp[0] == 'd' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_1.DUN;
			else if (temp[0] == 'd' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_1.DUO;
			else if (temp[0] == 'f' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.FAN;
			else if (temp[0] == 'f' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_1.FEI;
			else if (temp[0] == 'f' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_1.FEN;
			else if (temp[0] == 'f' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_1.FOU;
			else if (temp[0] == 'g' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_1.GAI;
			else if (temp[0] == 'g' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.GAN;
			else if (temp[0] == 'g' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_1.GAO;
			else if (temp[0] == 'g' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_1.GEI;
			else if (temp[0] == 'g' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_1.GEN;
			else if (temp[0] == 'g' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_1.GOU;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'a')
				result = CharsLibrary_1.GUA;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_1.GUI;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_1.GUN;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_1.GUO;
			else if (temp[0] == 'h' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_1.HAI;
			else if (temp[0] == 'h' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.HAN;
			else if (temp[0] == 'h' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_1.HAO;
			else if (temp[0] == 'h' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_1.HEI;
			else if (temp[0] == 'h' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_1.HEN;
			else if (temp[0] == 'h' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_1.HOU;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'a')
				result = CharsLibrary_1.HUA;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_1.HUI;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_1.HUN;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_1.HUO;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'a')
				result = CharsLibrary_1.JIA;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_1.JIE;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_1.JIN;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_1.JIU;
			else if (temp[0] == 'j' && temp[1] == 'u' && temp[2] == 'e')
				result = CharsLibrary_1.JUE;
			else if (temp[0] == 'j' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_1.JUN;
			else if (temp[0] == 'k' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_1.KAI;
			else if (temp[0] == 'k' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_1.KAN;
			else if (temp[0] == 'k' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_1.KAO;
			else if (temp[0] == 'k' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_1.KEN;
			else if (temp[0] == 'k' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_1.KOU;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'a')
				result = CharsLibrary_1.KUA;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_1.KUI;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_1.KUN;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_1.KUO;
			else if (temp[0] == 'l' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_2.LAI;
			else if (temp[0] == 'l' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_2.LAN;
			else if (temp[0] == 'l' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_2.LAO;
			else if (temp[0] == 'l' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_2.LEI;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'a')
				result = CharsLibrary_2.LIA;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_2.LIE;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_2.LIN;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_2.LIU;
			else if (temp[0] == 'l' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_2.LOU;
			else if (temp[0] == 'l' && temp[1] == 'u' && temp[2] == 'e')
				result = CharsLibrary_2.LUE;
			else if (temp[0] == 'l' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_2.LUN;
			else if (temp[0] == 'l' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_2.LUO;
			else if (temp[0] == 'm' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_2.MAI;
			else if (temp[0] == 'm' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_2.MAN;
			else if (temp[0] == 'm' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_2.MAO;
			else if (temp[0] == 'm' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_2.MEI;
			else if (temp[0] == 'm' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_2.MEN;
			else if (temp[0] == 'm' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_2.MIE;
			else if (temp[0] == 'm' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_2.MIN;
			else if (temp[0] == 'm' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_2.MIU;
			else if (temp[0] == 'm' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_2.MOU;
			else if (temp[0] == 'n' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_2.NAI;
			else if (temp[0] == 'n' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_2.NAN;
			else if (temp[0] == 'n' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_2.NAO;
			else if (temp[0] == 'n' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_2.NEI;
			else if (temp[0] == 'n' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_2.NEN;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_2.NIE;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_2.NIN;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_2.NIU;
			else if (temp[0] == 'n' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_2.NOU;
			else if (temp[0] == 'n' && temp[1] == 'u' && temp[2] == 'e')
				result = CharsLibrary_2.NUE;
			else if (temp[0] == 'n' && temp[1] == 'v' && temp[2] == 'e')
				result = CharsLibrary_2.NVE;
			else if (temp[0] == 'n' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_2.NUO;
			else if (temp[0] == 'p' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_2.PAI;
			else if (temp[0] == 'p' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_2.PAN;
			else if (temp[0] == 'p' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_2.PAO;
			else if (temp[0] == 'p' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_2.PEI;
			else if (temp[0] == 'p' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_2.PEN;
			else if (temp[0] == 'p' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_2.PIE;
			else if (temp[0] == 'p' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_2.PIN;
			else if (temp[0] == 'p' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_2.POU;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'a')
				result = CharsLibrary_2.QIA;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_2.QIE;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_2.QIN;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_2.QIU;
			else if (temp[0] == 'q' && temp[1] == 'u' && temp[2] == 'e')
				result = CharsLibrary_2.QUE;
			else if (temp[0] == 'q' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_2.QUN;
			else if (temp[0] == 'r' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_2.RAN;
			else if (temp[0] == 'r' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_2.RAO;
			else if (temp[0] == 'r' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_2.REN;
			else if (temp[0] == 'r' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_2.ROU;
			else if (temp[0] == 'r' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_2.RUI;
			else if (temp[0] == 'r' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_2.RUN;
			else if (temp[0] == 'r' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_2.RUO;
			else if (temp[0] == 's' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_2.SAI;
			else if (temp[0] == 's' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_2.SAN;
			else if (temp[0] == 's' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_2.SAO;
			else if (temp[0] == 's' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_2.SEN;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'a')
				result = CharsLibrary_2.SHA;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'e')
				result = CharsLibrary_2.SHE;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'i')
				result = CharsLibrary_2.SHI;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u')
				result = CharsLibrary_3.SHU;
			else if (temp[0] == 's' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_3.SOU;
			else if (temp[0] == 's' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_3.SUI;
			else if (temp[0] == 's' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_3.SUN;
			else if (temp[0] == 's' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_3.SUO;
			else if (temp[0] == 't' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_3.TAI;
			else if (temp[0] == 't' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_3.TAN;
			else if (temp[0] == 't' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_3.TAO;
			else if (temp[0] == 't' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_3.TIE;
			else if (temp[0] == 't' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_3.TOU;
			else if (temp[0] == 't' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_3.TUI;
			else if (temp[0] == 't' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_3.TUN;
			else if (temp[0] == 't' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_3.TUO;
			else if (temp[0] == 'w' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_3.WAI;
			else if (temp[0] == 'w' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_3.WAN;
			else if (temp[0] == 'w' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_3.WEI;
			else if (temp[0] == 'w' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_3.WEN;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'a')
				result = CharsLibrary_3.XIA;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'e')
				result = CharsLibrary_3.XIE;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_3.XIN;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'u')
				result = CharsLibrary_3.XIU;
			else if (temp[0] == 'x' && temp[1] == 'u' && temp[2] == 'e')
				result = CharsLibrary_3.XUE;
			else if (temp[0] == 'x' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_3.XUN;
			else if (temp[0] == 'y' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_3.YAN;
			else if (temp[0] == 'y' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_3.YAO;
			else if (temp[0] == 'y' && temp[1] == 'i' && temp[2] == 'n')
				result = CharsLibrary_3.YIN;
			else if (temp[0] == 'y' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_3.YOU;
			else if (temp[0] == 'y' && temp[1] == 'u' && temp[2] == 'e')
				result = CharsLibrary_3.YUE;
			else if (temp[0] == 'y' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_3.YUN;
			else if (temp[0] == 'z' && temp[1] == 'a' && temp[2] == 'i')
				result = CharsLibrary_3.ZAI;
			else if (temp[0] == 'z' && temp[1] == 'a' && temp[2] == 'n')
				result = CharsLibrary_3.ZAN;
			else if (temp[0] == 'z' && temp[1] == 'a' && temp[2] == 'o')
				result = CharsLibrary_3.ZAO;
			else if (temp[0] == 'z' && temp[1] == 'e' && temp[2] == 'i')
				result = CharsLibrary_3.ZEI;
			else if (temp[0] == 'z' && temp[1] == 'e' && temp[2] == 'n')
				result = CharsLibrary_3.ZEN;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'a')
				result = CharsLibrary_3.ZHA;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'e')
				result = CharsLibrary_3.ZHE;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'i')
				result = CharsLibrary_3.ZHI;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u')
				result = CharsLibrary_3.ZHU;
			else if (temp[0] == 'z' && temp[1] == 'o' && temp[2] == 'u')
				result = CharsLibrary_3.ZOU;
			else if (temp[0] == 'z' && temp[1] == 'u' && temp[2] == 'i')
				result = CharsLibrary_3.ZUI;
			else if (temp[0] == 'z' && temp[1] == 'u' && temp[2] == 'n')
				result = CharsLibrary_3.ZUN;
			else if (temp[0] == 'z' && temp[1] == 'u' && temp[2] == 'o')
				result = CharsLibrary_3.ZUO;
		} else if (length == 4) {
			if (temp[0] == 'b' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.BANG;
			else if (temp[0] == 'b' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.BENG;
			else if (temp[0] == 'b' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.BIAN;
			else if (temp[0] == 'b' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_1.BIAO;
			else if (temp[0] == 'b' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.BING;
			else if (temp[0] == 'c' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.CANG;
			else if (temp[0] == 'c' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.CENG;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'i')
				result = CharsLibrary_1.CHAI;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.CHAN;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_1.CHAO;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'e'
					&& temp[3] == 'n')
				result = CharsLibrary_1.CHEN;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'o'
					&& temp[3] == 'u')
				result = CharsLibrary_1.CHOU;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'i')
				result = CharsLibrary_1.CHUI;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'n')
				result = CharsLibrary_1.CHUN;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'o')
				result = CharsLibrary_1.CHUO;
			else if (temp[0] == 'c' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.CONG;
			else if (temp[0] == 'c' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.CUAN;
			else if (temp[0] == 'd' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.DANG;
			else if (temp[0] == 'd' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.DENG;
			else if (temp[0] == 'd' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.DIAN;
			else if (temp[0] == 'd' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_1.DIAO;
			else if (temp[0] == 'd' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.DING;
			else if (temp[0] == 'd' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.DONG;
			else if (temp[0] == 'd' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.DUAN;
			else if (temp[0] == 'f' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.FANG;
			else if (temp[0] == 'f' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.FENG;
			else if (temp[0] == 'g' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.GANG;
			else if (temp[0] == 'g' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.GENG;
			else if (temp[0] == 'g' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.GONG;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'i')
				result = CharsLibrary_1.GUAI;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.GUAN;
			else if (temp[0] == 'h' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.HANG;
			else if (temp[0] == 'h' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.HENG;
			else if (temp[0] == 'h' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.HONG;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'i')
				result = CharsLibrary_1.HUAI;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.HUAN;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.JIAN;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_1.JIAO;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.JING;
			else if (temp[0] == 'j' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.JUAN;
			else if (temp[0] == 'k' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.KANG;
			else if (temp[0] == 'k' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.KENG;
			else if (temp[0] == 'k' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_1.KONG;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'i')
				result = CharsLibrary_1.KUAI;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_1.KUAN;
			else if (temp[0] == 'l' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.LANG;
			else if (temp[0] == 'l' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.LENG;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.LIAN;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_2.LIAO;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.LING;
			else if (temp[0] == 'l' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.LONG;
			else if (temp[0] == 'l' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.LUAN;
			else if (temp[0] == 'm' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.MANG;
			else if (temp[0] == 'm' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.MENG;
			else if (temp[0] == 'm' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.MIAN;
			else if (temp[0] == 'm' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_2.MIAO;
			else if (temp[0] == 'm' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.MING;
			else if (temp[0] == 'n' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.NANG;
			else if (temp[0] == 'n' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.NENG;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.NIAN;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_2.NIAO;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.NING;
			else if (temp[0] == 'n' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.NONG;
			else if (temp[0] == 'n' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.NUAN;
			else if (temp[0] == 'p' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.PANG;
			else if (temp[0] == 'p' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.PENG;
			else if (temp[0] == 'p' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.PIAN;
			else if (temp[0] == 'p' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_2.PIAO;
			else if (temp[0] == 'p' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.PING;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.QIAN;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_2.QIAO;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.QING;
			else if (temp[0] == 'q' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.QUAN;
			else if (temp[0] == 'r' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.RANG;
			else if (temp[0] == 'r' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.RENG;
			else if (temp[0] == 'r' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.RONG;
			else if (temp[0] == 'r' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.RUAN;
			else if (temp[0] == 's' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.SANG;
			else if (temp[0] == 's' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_2.SENG;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'i')
				result = CharsLibrary_2.SHAI;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_2.SHAN;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_2.SHAO;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'e'
					&& temp[3] == 'n')
				result = CharsLibrary_2.SHEN;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'o'
					&& temp[3] == 'u')
				result = CharsLibrary_2.SHOU;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a')
				result = CharsLibrary_3.SHUA;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'i')
				result = CharsLibrary_3.SHUI;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'n')
				result = CharsLibrary_3.SHUN;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'o')
				result = CharsLibrary_3.SHUO;
			else if (temp[0] == 's' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.SONG;
			else if (temp[0] == 's' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.SUAN;
			else if (temp[0] == 't' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.TANG;
			else if (temp[0] == 't' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.TENG;
			else if (temp[0] == 't' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.TIAN;
			else if (temp[0] == 't' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_3.TIAO;
			else if (temp[0] == 't' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.TING;
			else if (temp[0] == 't' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.TONG;
			else if (temp[0] == 't' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.TUAN;
			else if (temp[0] == 'w' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.WANG;
			else if (temp[0] == 'w' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.WENG;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.XIAN;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_3.XIAO;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.XING;
			else if (temp[0] == 'x' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.XUAN;
			else if (temp[0] == 'y' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.YANG;
			else if (temp[0] == 'y' && temp[1] == 'i' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.YING;
			else if (temp[0] == 'y' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.YONG;
			else if (temp[0] == 'y' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.YUAN;
			else if (temp[0] == 'z' && temp[1] == 'a' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.ZANG;
			else if (temp[0] == 'z' && temp[1] == 'e' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.ZENG;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'i')
				result = CharsLibrary_3.ZHAI;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.ZHAN;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'o')
				result = CharsLibrary_3.ZHAO;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'e'
					&& temp[3] == 'n')
				result = CharsLibrary_3.ZHEN;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'o'
					&& temp[3] == 'u')
				result = CharsLibrary_3.ZHOU;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a')
				result = CharsLibrary_3.ZHUA;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'i')
				result = CharsLibrary_3.ZHUI;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'n')
				result = CharsLibrary_3.ZHUN;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'o')
				result = CharsLibrary_3.ZHUO;
			else if (temp[0] == 'z' && temp[1] == 'o' && temp[2] == 'n'
					&& temp[3] == 'g')
				result = CharsLibrary_3.ZONG;
			else if (temp[0] == 'z' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n')
				result = CharsLibrary_3.ZUAN;
		} else if (length == 5) {
			if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.CHANG;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'e'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.CHENG;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'o'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.CHONG;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'i')
				result = CharsLibrary_1.CHUAI;
			else if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'n')
				result = CharsLibrary_1.CHUAN;
			else if (temp[0] == 'g' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.GUANG;
			else if (temp[0] == 'h' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.HUANG;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.JIANG;
			else if (temp[0] == 'j' && temp[1] == 'i' && temp[2] == 'o'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.JIONG;
			else if (temp[0] == 'k' && temp[1] == 'u' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_1.KUANG;
			else if (temp[0] == 'l' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_2.LIANG;
			else if (temp[0] == 'n' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_2.NIANG;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_2.QIANG;
			else if (temp[0] == 'q' && temp[1] == 'i' && temp[2] == 'o'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_2.QIONG;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_2.SHANG;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'e'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_2.SHENG;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'i')
				result = CharsLibrary_3.SHUAI;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'n')
				result = CharsLibrary_3.SHUAN;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_3.XIANG;
			else if (temp[0] == 'x' && temp[1] == 'i' && temp[2] == 'o'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_3.XIONG;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'a'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_3.ZHANG;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'e'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_3.ZHENG;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'o'
					&& temp[3] == 'n' && temp[4] == 'g')
				result = CharsLibrary_3.ZHONG;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'i')
				result = CharsLibrary_3.ZHUAI;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'n')
				result = CharsLibrary_3.ZHUAN;
		} else if (length == 6) {
			if (temp[0] == 'c' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'n' && temp[5] == 'g')
				result = CharsLibrary_1.CHUANG;
			else if (temp[0] == 's' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'n' && temp[5] == 'g')
				result = CharsLibrary_3.SHUANG;
			else if (temp[0] == 'z' && temp[1] == 'h' && temp[2] == 'u'
					&& temp[3] == 'a' && temp[4] == 'n' && temp[5] == 'g')
				result = CharsLibrary_3.ZHUANG;
		}
		return result;
	}
}
