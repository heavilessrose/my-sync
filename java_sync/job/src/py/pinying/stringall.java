/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */

package py.pinying;

import java.io.*;

public class stringall implements Serializable {
	private java.util.Hashtable Hashdate;
	private java.util.Hashtable haallSomeSM = new java.util.Hashtable();
	private char[] separator = new char[] { (char) 001, (char) 002 };

	// static final long serialVersionUID=-168168168168168L;
	public stringall(java.util.Hashtable Hashdates) {
		Hashdate = Hashdates;
		this.haallSomeSM = new java.util.Hashtable();
		this.separator = new char[] { (char) 001, (char) 002 };
	}

	// public void ToObj(stringall stringall) {
	// try{
	// java.io.FileOutputStream fileout = new
	// java.io.FileOutputStream("c:/wmwpy.properties"); //
	// java.io.ObjectOutputStream objout = new
	// java.io.ObjectOutputStream(fileout);
	// objout.writeObject(stringall);
	// objout.flush();
	// objout.close();
	// }catch(Exception ee){ee.printStackTrace();}
	// }
	// 根据汉字得到拼音(包括多音)
	public String getpinying(String inhanzhi) {
		if (Hashdate.get(inhanzhi) == null) {
			return inhanzhi;
		} else {
			return Hashdate.get(inhanzhi).toString();
		}
	}

	// 根据汉字得到声母(包括多音)
	private String getShengmu(String inhanzhi) {
		if (inhanzhi == null || inhanzhi.equals("")) {
			return "";
		}

		String re = getpinying(inhanzhi);
		if (re == null || re.equals("")) {
			return inhanzhi;
		}
		StringBuffer getBuffer = new StringBuffer(re);
		int i0 = 0;
		int i1 = 0;
		String sre = "";
		for (int i = 0; i < getBuffer.length(); i++) {
			char cc = getBuffer.charAt(i);
			if (i1 == 1) {
				sre = sre + getBuffer.charAt(i);
				i0 = 1;
			}
			if (cc == '[') {
				i1 = 1;
				i0 = 0;
			} else {
				i1 = 0;
			}
			if (cc == ']') {
				i0 = 0;
			}

			if (i0 == 0) {
				sre = sre + getBuffer.charAt(i);
				i0 = 1;
			}
		}

		return sre;
	}

	public String[] getListSomepiny(String[] inString) {
		String res[] = new String[inString.length];
		for (int k = 0; k < inString.length; k++) {
			res[k] = getSomepiny(inString[k]);
		}
		return res;
	}

	public String getStringpiny(String inString) {
		return getSomepiny(inString).replace(separator[0], ' ').replaceAll(
				"" + separator[1], "");
	}

	private String getSomepiny(String inString) {
		String temp = "";
		String sreturn = "";
		if (inString == null || inString.equals("")) {
			return "";
		}
		for (int k = 0; k < inString.length(); k++) {
			temp = inString.substring(k, k + 1);
			if (temp.getBytes().length == 2) {
				sreturn = sreturn + separator[0] + getpinying(temp)
						+ separator[1];
			} else {
				sreturn = sreturn + temp;
			}

		}
		return sreturn;
	}

	public String[] getSomeSM(String[] inString) {
		String sarray[] = new String[inString.length];
		for (int k = 0; k < inString.length; k++) {
			sarray[k] = getSomeSM(inString[k]);
		}
		return sarray;
	}

	public String[] getListByHanzi(String[] inString, String piny) {
		String sarray[] = getSomeSM(inString);
		int tre[] = getIndexByPiny(sarray, piny);
		String rearray[] = new String[tre.length];
		for (int k = 0; k < tre.length; k++) {
			rearray[k] = inString[tre[k]];
		}
		return rearray;
	}

	public String[] getListByPiny(String[] inString, String[] py, String piny) {
		String sarray[] = py;
		int tre[] = getIndexByPiny(sarray, piny);
		String rearray[] = new String[tre.length];
		for (int k = 0; k < tre.length; k++) {
			rearray[k] = inString[tre[k]];
		}
		return rearray;
	}

	public String[] getListByPiny(String[] inString, String piny) {
		String sarray[] = inString;
		String resarray[] = new String[inString.length];
		java.util.Vector vector = new java.util.Vector();
		java.util.Vector rsvector = new java.util.Vector();
		if (piny == null || piny.equals("")) {
			return inString;
		}

		// 拼音 char
		char cpy[];
		if (piny.length() > 10) {
			cpy = new char[10];
		} else {
			cpy = new char[piny.length()];
		}

		piny.getChars(0, cpy.length, cpy, 0);

		StringBuffer getBuffer;
		int comp[] = new int[9];
		for (int i = 0; i < sarray.length; i++) {
			getBuffer = new StringBuffer(sarray[i]);
			boolean canadd = true;
			int iadd = 0;
			long bigtemp = 0;
			long big000 = 1;
			for (int ipiny = 0; ipiny < cpy.length; ipiny++) {
				int itemp = sarray[i].indexOf(cpy[ipiny]); // 出现在第几位
				if (itemp >= 0) {
					int huhao = getCount(getBuffer, itemp);
					if (huhao > 0) {
						itemp = itemp - huhao * 3;
					}
					itemp = 99 - itemp;

					big000 = 1;
					for (int c = 1; c < 9 - ipiny; c++) {
						big000 = big000 * 100;
					}
					bigtemp = bigtemp + itemp * big000;
				}
			}
			// 比较
			java.lang.Long objbigtemp = new java.lang.Long(bigtemp);
			if (vector.size() == 0) {
				vector.add(objbigtemp);
				rsvector.add(inString[i]);
				continue;
			}

			for (int kline = 0; kline < vector.size(); kline++) {
				long kone = new java.lang.Long(vector.elementAt(kline)
						.toString()).longValue();
				if (kone <= bigtemp && vector.size() == 1) {
					vector.insertElementAt(objbigtemp, 0);
					rsvector.insertElementAt(inString[i], 0);
					break;
				} else if (vector.size() == 1) {
					vector.add(objbigtemp);
					rsvector.add(inString[i]);
					break;
				}
				if (kline == vector.size() - 1) {
					vector.add(objbigtemp);
					rsvector.add(inString[i]);
					break;
				}
				long kone2 = new java.lang.Long(vector.elementAt(kline + 1)
						.toString()).longValue();

				if (kone2 <= bigtemp && bigtemp >= kone) {
					vector.insertElementAt(objbigtemp, kline);
					rsvector.insertElementAt(inString[i], kline);
					break;
				}
			}

		}
		for (int ire = 0; ire < rsvector.size(); ire++) {
			resarray[ire] = rsvector.elementAt(ire).toString();
		}
		return resarray;

	}

	public int compare(int[] oneFind, int[] oneFind2, String[] ins) {
		int re = 0;// System.out.println("oneFind  ");
		for (int k = 0; k < oneFind.length - 1; k++) {
			if (oneFind[k] >= 0 && oneFind2[k] == -1) {
				re = 1;
				break;
			}
			if (oneFind[k] == -1 && oneFind2[k] > -1) {
				re = -1;
				break;
			}
			if (oneFind[k] < oneFind2[k]) {
				re = 1;
				break;
			} else if (oneFind[k] > oneFind2[k]) {
				re = -1;
				break;
			}
		}
		int Fcount = 0;
		int Scount = 0;
		for (int k = 0; k < oneFind.length - 2; k++) {
			if (oneFind[k] - oneFind[k + 1] == -1) {
				Fcount++;
			}
			if (oneFind2[k] - oneFind2[k + 1] == -1) {
				Scount++;
			}
		}
		if (Fcount > Scount) {
			re = 1;
		}
		if (Fcount < Scount) {
			re = -1;
		}
		if (re == 0) {
			re = ins[oneFind[oneFind.length - 1]]
					.compareTo(ins[oneFind2[oneFind2.length - 1]]);
		}
		return re;
	}

	public void copy(int[] i, int[] j) {
		for (int k = 0; k < i.length; k++) {
			j[k] = i[k];

		}

	}

	public synchronized int[] getIndexByPiny(String[] inString, String piny) {
		System.out.println("arrange:" + piny);
		String sarray[] = inString;
		piny = piny.toLowerCase();
		int resarray[] = new int[inString.length];
		java.util.Vector vector = new java.util.Vector();
		java.util.Vector rsvector = new java.util.Vector();
		if (piny == null || piny.equals("")) {
			return resarray;
		}
		int[][] AoneFind = new int[sarray.length][piny.length() + 1];
		int[] compan = new int[piny.length() + 1];
		// vector.setSize( sarray.length);rsvector.setSize( sarray.length);
		for (int i = 0; i < sarray.length; i++) {
			int seekinString = 0;
			int seekpiny = 0;
			String oneIN = sarray[i].toLowerCase();
			String stmp = "";
			// AoneFind[i]=new int[];
			for (int ip = 0; ip < piny.length(); ip++) {
				AoneFind[i][ip] = -1;
			}
			AoneFind[i][piny.length()] = i;
			for (int k = 0; k < oneIN.length(); k++) {
				if (seekpiny == piny.length()) {
					break;
				}
				if (oneIN.substring(k, k + 1).charAt(0) == separator[0]) {
					int endH = oneIN.indexOf(separator[1], k);
					stmp = oneIN.substring(k, endH);
					if (stmp.indexOf(piny.substring(seekpiny, seekpiny + 1)) >= 0) {

						AoneFind[i][seekpiny] = seekinString;
						seekpiny++;
					}
					k = endH;
					seekinString++;
				} else {
					stmp = oneIN.substring(k, k + 1);
					if (stmp.indexOf(piny.substring(seekpiny, seekpiny + 1)) >= 0) {
						AoneFind[i][seekpiny] = seekinString;
						seekpiny++;
					}
					seekinString++;
				}
			}
			// System.out.println(sarray[i] + " " + AoneFind[i][0] + " " +
			// AoneFind[i].toString() + " " + piny);
		}

		for (int k1 = 0; k1 < AoneFind.length; k1++) {
			for (int k2 = k1 + 1; k2 < AoneFind.length; k2++) {
				int cw = compare(AoneFind[k2], AoneFind[k1], sarray);
				if (cw > 0) {
					copy(AoneFind[k2], compan);// compan =AoneFind[k2];
					copy(AoneFind[k1], AoneFind[k2]);
					copy(compan, AoneFind[k1]);
				}
			}
		}
		// if (i == 0) {
		// vector.setElementAt(AoneFind[i],0);
		// rsvector.setElementAt(new java.lang.Integer(i),0);
		// continue;
		// }
		//
		// for (int kline = 0; kline < vector.size(); kline++) {
		// if(vector.elementAt(kline)==null){
		// vector.setElementAt(oneFind,vector.size());
		// rsvector.setElementAt(new java.lang.Integer(i),vector.size());
		// break;
		//
		// }
		// int[] kone =(int[])vector.elementAt(kline);
		// int cw= compare(oneFind,kone);
		// if(cw==0){
		// int
		// y=java.lang.Integer.parseInt(rsvector.elementAt(kline).toString().trim());
		// cw= oneIN.compareTo(inString[y]);
		// }
		// if (cw>=0 && kline== 0) {
		// vector.insertElementAt(oneFind, 0);
		// rsvector.insertElementAt(new java.lang.Integer(i), 0);
		// break;
		// }
		// else if (cw<=0 &&vector.size() == 1) {
		// vector.setElementAt(oneFind,vector.size());
		// rsvector.setElementAt(new java.lang.Integer(i),vector.size());
		// vector.add(oneFind);
		// rsvector.add(new java.lang.Integer(i));
		// break;
		// }
		// int[] kone2 =(int[])vector.elementAt(kline + 1);
		// int cw2= compare(oneFind,kone2);
		// if(cw2==0){
		// int
		// y=java.lang.Integer.parseInt(rsvector.elementAt(kline+1).toString().trim());
		// cw2= oneIN.compareTo(inString[y]);
		// }
		// if (cw2>0&&cw<0) {
		// vector.insertElementAt(oneFind, kline);
		// rsvector.insertElementAt(new java.lang.Integer(i), kline+1);
		// break;
		// }
		// }

		for (int ire = 0; ire < AoneFind.length; ire++) {
			try {
				resarray[ire] = AoneFind[ire][AoneFind[0].length - 1];
			} catch (Exception eer) {
				eer.printStackTrace();
				System.out.println(" " + ire);
			}

		}
		return resarray;

	}

	public int getCount(StringBuffer getBuffer, int itemp) {
		int count = 0;
		for (int k = 0; k < itemp; k++) {
			if (getBuffer.charAt(k) == '[') {
				count++;
			}
		}
		return count;
	}

	public String getStringSM(String inString) {
		return getSomeSM(inString).replace(separator[0], ' ').replaceAll(
				"" + separator[1], "");
	}

	private String getSomeSM(String inString) {
		if (haallSomeSM.get(inString) != null) {
			return haallSomeSM.get(inString).toString();
		}
		String temp = "";
		if (inString == null || inString.equals("")) {
			return "";
		}
		for (int k = 0; k < inString.length(); k++) {
			if ((inString.substring(k, k + 1)).getBytes().length == 2) {
				temp = temp + separator[0]
						+ getShengmu(inString.substring(k, k + 1))
						+ separator[1];
			} else {
				temp = temp + inString.substring(k, k + 1);
			}
		}
		haallSomeSM.put(inString, temp);
		return temp;
	}

	public static void main(String[] args) {
		// stringall stringall1 = new stringall();
	}

}
