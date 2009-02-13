/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db.index;

import bm.db.DBException;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import java.util.Vector;
import java.io.InputStream;

import bm.storage.RSException;

import bm.storage.RecordStoreFullException;

/*
 * File Information
 *
 * Created on       : 07-jul-2005 11:24:43
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2006-09-15 13:32:30 +0200 (vie, 15 sep 2006) $
 * Revision         : $Revision:10 $
 */

/**
 * Test suite for Index
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:10 $
 * @noinspection OverlyLargePrimitiveArrayInitializer,ArrayLengthInLoopCondition,MethodCallInLoopCondition,FieldRepeatedlyAccessedInMethod
 */
public class IndexTest
        extends TestCase
{
    private static Index index = new Index( "TEST_INDEX", 2, Index.KT_LONG, false );
    private static final long[] BULK_TEST_KEYS = new long[]{ -12L,
     2113L, -1271L, 1728L,
     1057L, -650L, -2571L,
     -2544L, 1959L, 151L,
     2056L, -1897L, -2531L,
     -1641L, 2427L, 1497L,
     2649L, -1622L, -1146L,
     2828L, 2987L, 2251L,
     514L, 2541L, 1607L,
     550L, -17L, -748L,
     -1257L, -2194L, -1905L,
     -1101L, -2369L, -2112L,
     2609L, 1363L, -1145L,
     798L, -2153L, 78L,
     -139L, -466L, 2312L,
     -1106L, 2368L, 217L,
     -2497L, -347L, 1206L,
     1788L, -1422L, 515L,
     -2705L, -1526L, 324L,
     388L, 1543L, -137L,
     1427L, -1873L, -1224L,
     -2921L, -2950L, 904L,
     2694L, 229L, -2632L,
     1062L, -1823L, 2148L,
     1592L, 1238L, 2126L,
     2370L, 1695L, 1870L,
     734L, -1448L, -15L,
     -1757L, -2826L, -1325L,
     2445L, -485L, -1922L,
     -2450L, -2380L, 1860L,
     2674L, -1178L, 358L,
     -1709L, -1780L, 2556L,
     1381L, 2638L, -649L,
     375L, -1585L, -5L,
     -2778L, 2959L, -1888L,
     -610L, 2787L, 227L,
     1662L, 1719L, -1472L,
     -1028L, -2565L, 321L,
     2082L, -1440L, -569L,
     -2395L, -2344L, 888L,
     2188L, 1645L, 256L,
     866L, -1656L, -647L,
     2675L, 1559L, -490L,
     -747L, 1461L, 1134L,
     1253L, -2231L, -98L,
     128L, 1183L, 2112L,
     -2357L, -1702L, -900L,
     -1486L, 765L, -1354L,
     -1155L, -1278L, -520L,
     -2566L, -2343L, 2672L,
     134L, 2975L, -1397L,
     2423L, 322L, -2029L,
     -648L, 2024L, 1422L,
     2168L, 31L, -866L,
     -2283L, 146L, -1150L,
     24L, 2473L, 1261L,
     -722L, -1909L, 1166L,
     -195L, 945L, -2315L,
     -739L, -27L, 2421L,
     -885L, -1926L, 2505L,
     -1294L, 2940L, 399L,
     899L, -1583L, -2109L,
     -185L, -735L, 1074L,
     -2536L, 787L, 917L,
     -209L, 2928L, 817L,
     -380L, -2313L, 2065L,
     -929L, 160L, 2212L,
     -1801L, 820L, -738L,
     -2075L, 1850L, -976L,
     1129L, 1821L, -1795L,
     728L, -1547L, 2226L,
     -2975L, -2685L, 2116L,
     -1313L, 168L, -2553L,
     2061L, -1246L, 1522L,
     -2284L, -2712L, 598L,
     195L, 519L, 2603L,
     -1894L, -2501L, -191L,
     -2344L, 1457L, -2011L,
     2158L, 972L, 2120L,
     2440L, 1831L, 1234L,
     2420L, 1897L, 128L,
     -1770L, -2063L, 1519L,
     -2630L, -2597L, 1403L,
     1078L, -2358L, 142L,
     -2967L, 2477L, -2432L,
     272L, -1115L, -2456L,
     -14L, 1431L, -2635L,
     2510L, -2122L, -2667L,
     -459L, 291L, -2445L,
     1568L, -2861L, 0L,
     -1655L, -1383L, -2342L,
     -1778L, -124L, 2485L,
     -1328L, -1473L, 2357L,
     -889L, 991L, -1363L,
     -1607L, -1844L, -113L,
     1145L, -704L, 1085L,
     -2301L, -1883L, -1937L,
     2168L, 2832L, -1190L,
     -1457L, 1008L, 377L,
     2547L, 2803L, 2121L,
     2445L, 2904L, -378L,
     -1435L, 103L, 605L,
     -1357L, -1983L, 845L,
     -344L, -1869L, -2784L,
     -1070L, -1498L, -2285L,
     1063L, -2907L, -2938L,
     -573L, 591L, -2192L,
     -1900L, -784L, 583L,
     -2407L, -680L, 938L,
     1312L, -612L, 2949L,
     1183L, -1345L, 103L,
     2090L, -618L, -1922L,
     724L, -2084L, -1219L,
     -1958L, 179L, -1718L,
     1988L, 2654L, 741L,
     -160L, 2560L, 1840L,
     802L, -235L, -751L,
     2711L, 748L, -756L,
     2907L, -2901L, 1074L,
     1943L, 1785L, 2717L,
     -2490L, -281L, 1189L,
     -2166L, 487L, 1845L,
     2642L, 1324L, -272L,
     1125L, 1371L, -79L,
     2479L, 2342L, -768L,
     -2062L, -2521L, -1317L,
     -2456L, -91L, 2054L,
     -2620L, -150L, 1044L,
     504L, -1106L, -2125L,
     604L, -1926L, 55L,
     -1949L, -1537L, 1863L,
     2603L, 2959L, -2262L,
     -593L, 1210L, 1089L,
     192L, -2357L, 2567L,
     -2861L, -2027L, 418L,
     -646L, -1472L, 835L,
     1848L, -409L, -220L,
     -1224L, 1745L, -2939L,
     1494L, 2750L, -1238L,
     -2655L, -459L, 1635L,
     1993L, -864L, -2848L,
     -1234L, 448L, -1160L,
     -1651L, -40L, 102L,
     -1057L, -269L, 40L,
     -179L, 1466L, 2143L,
     2990L, -547L, 1000L,
     -1733L, -2711L, -2288L,
     -2475L, 1098L, -2055L,
     972L, 2008L, -2890L,
     2694L, -2204L, -2460L,
     -1269L, -1018L, 1895L,
     2435L, -2311L, -2445L,
     -2376L, -202L, -2771L,
     51L, -1964L, 2326L,
     2800L, -65L, 319L,
     1631L, -945L, -1644L,
     -1489L, 215L, -487L,
     -2091L, 71L, 2268L,
     2556L, -1562L, -1818L,
     -989L, -767L, 2427L,
     -2058L, 2402L, -2549L,
     -2897L, -2787L, -593L,
     2860L, -2979L, 1375L,
     -138L, 337L, 715L,
     1250L, -1346L, 2529L,
     -1854L, 584L, -1971L,
     -1087L, -2001L, 2121L,
     -2408L, 1435L, 2525L,
     1006L, -1900L, -1334L,
     1209L, -1489L, 2472L,
     1439L, -1587L, 2598L,
     1704L, 336L, 1797L,
     2018L, 2324L, 1092L,
     -2131L, 861L, 78L,
     2672L, 2041L, -2618L,
     1304L, -763L, -2501L,
     2272L, 356L, 1695L,
     -2832L, 2817L, 578L,
     1519L, -1429L, 2604L,
     557L, 682L, 2833L,
     -2864L, -2906L, -847L,
     -1164L, 2974L, -1184L,
     1967L, 2929L, 2918L,
     2730L, -2538L, -263L,
     1227L, 1408L, 1778L,
     1243L, -1670L, 2131L,
     460L, 752L, -320L,
     -2372L, 328L, -2666L,
     -431L, -1963L, 2943L,
     -1426L, -519L, 616L,
     1814L, -1817L, 919L,
     -2619L, -107L, 664L,
     -291L, 2657L, 762L,
     2169L, -2478L, 258L,
     1091L, -837L, 2032L,
     -164L, -2873L, -669L,
     -898L, -946L, -2842L,
     -2341L, -370L, -226L,
     -2076L, 2113L, -1600L,
     -2807L, 1330L, -2641L,
     2046L, -2572L, -1571L,
     1393L, -2138L, -890L,
     -1195L, -2214L, 99L,
     2163L, -2809L, -1129L,
     -1300L, 1202L, -2308L,
     -2366L, -516L, -1128L,
     1880L, -1992L, 120L,
     -2460L, -2532L, -2124L,
     -1780L, 328L, -2884L,
     -1998L, -1673L, -1178L,
     -583L, 156L, -1054L,
     -911L, 2872L, 2809L,
     -2588L, -392L, 2969L,
     9L, -983L, 2302L,
     657L, 2922L, 1542L,
     2733L, -2848L, -2242L,
     479L, 1126L, -2412L,
     899L, 2994L, -2158L,
     -2164L, 145L, 2961L,
     -92L, 2078L, -2376L,
     388L, 2669L, -2622L,
     2263L, -2595L, -529L,
     -1819L, 662L, -1358L,
     -1237L, -2481L, -667L,
     1750L, -605L, 397L,
     -861L, -608L, -368L,
     487L, -656L, 1834L,
     -1299L, 2723L, -1157L,
     2163L, -750L, -1791L,
     -44L, -1090L, 1666L,
     -2684L, 1027L, 1745L,
     -1427L, 1175L, -464L,
     -2281L, 679L, 462L,
     -2680L, -730L, -225L,
     1425L, 823L, -318L,
     1721L, 645L, 760L,
     80L, 500L, -1582L,
     2855L, 404L, -451L,
     -1954L, 559L, -581L,
     -1325L, -2448L, 836L,
     -574L, 1560L, -1570L,
     587L, 2557L, 1822L,
     -217L, 221L, -72L,
     2264L, 1722L, -1288L,
     674L, 1892L, 302L,
     -2875L, -2411L, -136L,
     -568L, 731L, -1224L,
     -326L, -2344L, 2363L,
     1829L, 1519L, 475L,
     2450L, -1581L, 2525L,
     170L, -2902L, -1428L,
     2200L, 2522L, 137L,
     -1832L, 569L, -2765L,
     -2712L, 321L, -110L,
     -2031L, -747L, 1357L,
     699L, -2794L, -2903L,
     -1472L, -911L, -746L,
     -2629L, 2111L, 1513L,
     201L, 2364L, 2997L,
     -1279L, 588L, 1481L,
     -1406L, 1768L, -2040L,
     -1260L, 327L, 1217L,
     -1534L, 2539L, 1667L,
     228L, -1998L, -288L,
     -1663L, 1693L, 2684L,
     -777L, -1808L, 27L,
     2561L, 715L, -2695L,
     146L, 311L, 649L,
     -683L, 1186L, -771L,
     891L, -1115L, 533L,
     1240L, -1522L, -656L,
     -1915L, 2001L, -2236L,
     2600L, -1724L, 2805L,
     -613L, -2187L, -215L,
     531L, 965L, 1664L,
     -438L, 2522L, -754L,
     -743L, 2237L, 2432L,
     2337L, 1674L, -2539L,
     1692L, -61L, -1438L,
     -1245L, 2821L, -1474L,
     1508L, -1693L, 368L,
     192L, 1037L, 123L,
     -34L, 2561L, 1601L,
     2983L, -1389L, 969L,
     -1370L, 2624L, -119L,
     -1473L, -233L, -280L,
     -1037L, -877L, -2566L,
     -561L, -1750L, -2403L,
     2291L, 33L, 284L,
     2027L, -273L, -2690L,
     1655L, 2735L, -390L,
     -599L, -517L, 847L,
     1209L, -2174L, 557L,
     12L, -527L, -1364L,
     -1371L, -93L, 2950L,
     2943L, 1921L, -1137L,
     -2595L, -1433L, -457L,
     1597L, 1266L, 339L,
     -1313L, -50L, -2878L,
     -673L, -2417L, 2240L,
     1227L, 330L, -251L,
     750L, -1809L, -1279L,
     -1216L, -917L, 1629L,
     554L, -2680L, 1323L,
     -752L, -86L, 289L,
     531L, -2826L, -2232L,
     373L, 260L, -1366L,
     -2313L, 2807L, -404L,
     -646L, -1952L, 2512L,
     924L, 1681L, -1411L,
     749L, -2189L, -178L,
     -8L, 727L, 289L,
     2092L, 866L, 2074L,
     -347L, -768L, -636L,
     1902L, -160L, 806L,
     625L, -1815L, -2680L,
     2805L, -1710L, 2508L,
     -2666L, -2322L, -767L,
     2993L, 2448L, -456L,
     926L, 844L, -216L,
     1055L, -2187L, -1L,
     506L, 2265L, -2338L,
     1459L, 1074L, 64L,
     -1756L, -610L, 932L,
     -942L, 2095L, 2888L,
     -2398L, 2132L, 138L,
     -1851L, 2802L, -2053L,
     -691L, 1718L, -2219L,
     -2639L, 1388L, 1960L,
     -2854L, -1874L, -1226L,
     2931L, -2216L, -1531L,
     255L, 596L, -1921L,
     -270L, 2862L, -965L,
     -2952L, -2397L, 2896L
     };
    private static Index bulk2 = new Index( "BULK_INDEX", 2, Index.KT_LONG, false );
    private static Index bulk8 = new Index( "BULK_INDEX8", 8, Index.KT_LONG, false );
    private static Index bulk50 = new Index( "BULK_INDEX50", 50, Index.KT_LONG, false );

    private static Vector runsInsert = new Vector( 10 );
    private static Vector runsRemove = new Vector( 10 );
    private final int[][] inserts = new int[][]{
            { 10, 10 },
            { 20, 20 },
            { 30, 30 },
            { 40, 40 },
            { 50, 50 },
            { 60, 60 },
            { 70, 70 },
            { 80, 80 },
            { 90, 90 },
            { 100, 100 },
            { 110, 110 },
            { 120, 120 },
            { 130, 130 },
            { 140, 140 },
            { 150, 150 },
            { 160, 160 },
            { 170, 170 },
            { 45, 45 }
    };
    private final NodeInfo[][] resultsInsert = new NodeInfo[][]{
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addChild( 2 )
                            .addChild( 3 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addChild( 2 )
                            .addChild( 3 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addChild( 2 )
                            .addChild( 3 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 )
                            .addChild( 5 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 )
                            .addChild( 5 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 )
                            .addChild( 5 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
                            .addKey( new NodeKeyInfo( 130, 130 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 )
                            .addChild( 5 )
                            .addChild( 6 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) ),
                    new NodeInfo( 6 )
                            .addKey( new NodeKeyInfo( 130, 130 ) )
                            .addKey( new NodeKeyInfo( 140, 140 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 )
                            .addChild( 5 )
                            .addChild( 6 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) ),
                    new NodeInfo( 6 )
                            .addKey( new NodeKeyInfo( 130, 130 ) )
                            .addKey( new NodeKeyInfo( 140, 140 ) )
                            .addKey( new NodeKeyInfo( 150, 150 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 )
                            .addChild( 5 )
                            .addChild( 6 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) ),
                    new NodeInfo( 6 )
                            .addKey( new NodeKeyInfo( 130, 130 ) )
                            .addKey( new NodeKeyInfo( 140, 140 ) )
                            .addKey( new NodeKeyInfo( 150, 150 ) )
                            .addKey( new NodeKeyInfo( 160, 160 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addChild( 8 )
                            .addChild( 9 ),
                    new NodeInfo( 8 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 ),
                    new NodeInfo( 9 )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
                            .addKey( new NodeKeyInfo( 150, 150 ) )
                            .addChild( 5 )
                            .addChild( 6 )
                            .addChild( 7 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) ),
                    new NodeInfo( 6 )
                            .addKey( new NodeKeyInfo( 130, 130 ) )
                            .addKey( new NodeKeyInfo( 140, 140 ) ),
                    new NodeInfo( 7 )
                            .addKey( new NodeKeyInfo( 160, 160 ) )
                            .addKey( new NodeKeyInfo( 170, 170 ) )
            },
            {
                    new NodeInfo( 1 )
                            .addKey( new NodeKeyInfo( 90, 90 ) )
                            .addChild( 8 )
                            .addChild( 9 ),
                    new NodeInfo( 8 )
                            .addKey( new NodeKeyInfo( 30, 30 ) )
                            .addKey( new NodeKeyInfo( 60, 60 ) )
                            .addChild( 2 )
                            .addChild( 3 )
                            .addChild( 4 ),
                    new NodeInfo( 9 )
                            .addKey( new NodeKeyInfo( 120, 120 ) )
                            .addKey( new NodeKeyInfo( 150, 150 ) )
                            .addChild( 5 )
                            .addChild( 6 )
                            .addChild( 7 ),
                    new NodeInfo( 2 )
                            .addKey( new NodeKeyInfo( 10, 10 ) )
                            .addKey( new NodeKeyInfo( 20, 20 ) ),
                    new NodeInfo( 3 )
                            .addKey( new NodeKeyInfo( 40, 40 ) )
                            .addKey( new NodeKeyInfo( 45, 45 ) )
                            .addKey( new NodeKeyInfo( 50, 50 ) ),
                    new NodeInfo( 4 )
                            .addKey( new NodeKeyInfo( 70, 70 ) )
                            .addKey( new NodeKeyInfo( 80, 80 ) ),
                    new NodeInfo( 5 )
                            .addKey( new NodeKeyInfo( 100, 100 ) )
                            .addKey( new NodeKeyInfo( 110, 110 ) ),
                    new NodeInfo( 6 )
                            .addKey( new NodeKeyInfo( 130, 130 ) )
                            .addKey( new NodeKeyInfo( 140, 140 ) ),
                    new NodeInfo( 7 )
                            .addKey( new NodeKeyInfo( 160, 160 ) )
                            .addKey( new NodeKeyInfo( 170, 170 ) )
            }
    };

    public IndexTest()
    {
        super();
    }

    public IndexTest( final String s)
    {
        super(s);
    }

    /**
     * Test case, inserting:
     * 1. insert 1,1 => 1 node, 1,1
     * 2. insert 10,2 => 1 node, 1,1;10,2
     * 3. insert 25,3 => 1 node, 1,1;10,2;25,3
     * 4. insert 4,4 => 1 node, 1,1;4,4;10,2;25,3
     * 5. insert 8,5 => 3 nodes, root[8,5], left[1,1;4,4], right[10,2;25,3]
     * 6. insert 23,6 => 3 nodes, root[8,5], left[1,1;4,4], right[10,2;23,6;25,3]
     * 7. insert 55,7 => 3 nodes, root[8,5], left[1,1;4,4], right[10,2;23,6;25,3;55,7]
     * 8. insert 90,8 => 4 nodes, root[8,5;25,3], child0[1,1;4,4], child1[10,2;23,6], child2[55,7;90,8]
     * 9. insert 9,9 => 4 nodes, root[8,5;25,3], child0[1,1;4,4], child1[9,9;10,2;23,6], child2[55,7;90,8]
     * 10. insert 7,10 => 4 nodes, root[8,5;25,3], child0[1,1;4,4;7,10], child1[9,9;10,2;23,6], child2[55,7;90,8]
     * 11. insert 8,11 => 4 nodes, root[8,5,11;25,3], child0[1,1;4,4;7,10], child1[9,9;10,2;23,6], child2[55,7;90,8]
     * 12. insert 9,9 => 4 nodes, root[8,5,11;25,3], child0[1,1;4,4;7,10], child1[9,9;10,2;23,6], child2[55,7;90,8]
     * 13. insert 5,12 => 4 nodes, root[8,5,11;25,3], child0[1,1;4,4;5,12;7,10], child1[9,9;10,2;23,6], child2[55,7;90,8]
     * 14. insert 2,13 => 5 nodes, root[4,4;8,5,11;25,3], child0[1,1;2,13], child1[5,12;7,10], child2[9,9;10,2;23,6], child3[55,7;90,8]
     * 15. insert 16,14 => 5 nodes, root[4,4;8,5,11;25,3], child0[1,1;2,13], child1[5,12;7,10], child2[9,9;10,2;16,14;23,6], child3[55,7;90,8]
     * 16. insert 15,15 => 6 nodes, root[4,4;8,5,11;15,15;25,3], child0[1,1;2,13], child1[5,12;7,10], child2[9,9;10,2], child3[16,14;23,6], child4[55,7;90,8]
     */
    public void testInsert()
    {
        final int[][] inputs = {
            {1,1},
            {10,2},
            {25,3},
            {4,4},
            {8,5},
            {23,6},
            {55,7},
            {90,8},
            {9,9},
            {7,10},
            {8,11},
            {9,9},
            {5,12},
            {2,13},
            {16,14},
            {15,15},
            {40,16},
            {45,17},
            {75,18}
        };
        final NodeInfo[][] results = {
            { // scenario 1
                new NodeInfo( 1 ).addKey( new NodeKeyInfo( 1, 1 ) )
            },
            { // scenario 2
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
            },
            { // scenario 3
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
            },
            { // scenario 4
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
            },
            { // scenario 5
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addChild( 2 )
                        .addChild( 3 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
            },
            { // scenario 6
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addChild( 2 )
                        .addChild( 3 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
            },
            { // scenario 7
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addChild( 2 )
                        .addChild( 3 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
            },
            { // scenario 8
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
            },
            { // scenario 9
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
            },
            { // scenario 10
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
            },
            { // scenario 11
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
            },
            { // scenario 12
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
            },
            { // scenario 13
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
            },
            { // scenario 14
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) )
            },
            { // scenario 15
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) )
                        .addKey( new NodeKeyInfo( 16, 14 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) )
            },
            { // scenario 16
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 16, 14 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
            },
            { // scenario 17 (insert 40,16)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 40, 16 ) )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 16, 14 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
            },
            { // scenario 18 (insert 45,17)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 4 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 40, 16 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 16, 14 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
            },
            { // scenario 19 (insert 75,18)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addChild( 8 )
                        .addChild( 9 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 40, 16 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 16, 14 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 8 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ).addValue( 11 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 ),
                new NodeInfo( 9 )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addChild( 6 )
                        .addChild( 4 )
                        .addChild( 7 )
            }
        };
        try
        {
            for( int i = 0; i < inputs.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.insert( new Long( inputs[i][0] ), inputs[i][1] );
                check( results[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private void check( final NodeInfo[] result )
            throws DBException,
                   RSException,
                   RecordStoreFullException
    {
        // Check number of nodes
        index.getRecordStore().open();
        assertEquals( result.length, index.getRecordStore().getNumRecords() );
        index.getRecordStore().close();

        for( int i = 0; i < result.length; i++ )
        {
            final NodeInfo nodeInfo = result[i];
            final Node node = index.getNode( null, nodeInfo.recordId );
            checkNode( nodeInfo, node );
        }
    }

    private void checkNode( final NodeInfo template, final Node node )
    {
        // Check key count
        assertEquals( template.keyInfo.size(), node.getKeyCount() );
        for( int i = 0; i < template.keyInfo.size(); i++ )
        {
            final NodeKey nodeKey = node.getNodeKey( i );
            checkKey(nodeKey, (NodeKeyInfo) template.keyInfo.elementAt( i ) );
        }
        if( template.children.size() > 0 )
        {
            assertEquals( template.children.size(), node.getKeyCount() + 1 );
            for( int i = 0; i < template.children.size(); i++ )
            {
                assertEquals(
                        ((Integer)template.children.elementAt(i)).intValue(),
                        node.getChild( i )
                );
            }
        }
    }

    private void checkKey( final NodeKey nodeKey, final NodeKeyInfo nodeKeyInfo )
    {
        assertNotNull( nodeKey );
        // check data size
        assertEquals( nodeKeyInfo.values.size(), ((SortedIntArray)nodeKey.getData()).size() );
        // check key
        assertEquals( nodeKeyInfo.key, nodeKey.getKey().longValue() );
        for( int i = 0; i < nodeKeyInfo.values.size(); i++ )
        {
            assertEquals(
                    ((Integer)nodeKeyInfo.values.elementAt( i )).intValue(),
                    ((SortedIntArray)nodeKey.getData()).getElementAt( i )
            );
        }
    }



    public void testFind()
    {
        try
        {
            int[] result = (int[]) index.find( new Long( 100 ) );
            assertNull( result );

            result = (int[]) index.find( new Long( 8 ) );
            assertNotNull( result );
            assertEquals( 2, result.length );
            assertEquals( 5, result[0] );
            assertEquals( 11, result[1] );

            result = (int[]) index.find( new Long( 4 ) );
            assertNotNull( result );
            assertEquals( 1, result.length );
            assertEquals( 4, result[0] );

            result = (int[]) index.find( new Long( 1 ) );
            assertNotNull( result );
            assertEquals( 1, result.length );
            assertEquals( 1, result[0] );

            result = (int[]) index.find( new Long( 7 ) );
            assertNotNull( result );
            assertEquals( 1, result.length );
            assertEquals( 10, result[0] );

            result = (int[]) index.find( new Long( 9 ) );
            assertNotNull( result );
            assertEquals( 1, result.length );
            assertEquals( 9, result[0] );

            result = (int[]) index.find( new Long( 16 ) );
            assertNotNull( result );
            assertEquals( 1, result.length );
            assertEquals( 14, result[0] );

            result = (int[]) index.find( new Long( 55 ) );
            assertNotNull( result );
            assertEquals( 1, result.length );
            assertEquals( 7, result[0] );
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testRemoveBorrowFromRight()
    {
        final int[][] deletes  = {
                { 10, 10 },
                { 20, 20 }
        };
        final NodeInfo[][] resultsDelete = {
                {
                        new NodeInfo( 1 )
                                .addKey( new NodeKeyInfo( 90, 90 ) )
                                .addChild( 8 )
                                .addChild( 9 ),
                        new NodeInfo( 8 )
                                .addKey( new NodeKeyInfo( 40, 40 ) )
                                .addKey( new NodeKeyInfo( 60, 60 ) )
                                .addChild( 2 )
                                .addChild( 3 )
                                .addChild( 4 ),
                        new NodeInfo( 9 )
                                .addKey( new NodeKeyInfo( 120, 120 ) )
                                .addKey( new NodeKeyInfo( 150, 150 ) )
                                .addChild( 5 )
                                .addChild( 6 )
                                .addChild( 7 ),
                        new NodeInfo( 2 )
                                .addKey( new NodeKeyInfo( 20, 20 ) )
                                .addKey( new NodeKeyInfo( 30, 30 ) ),
                        new NodeInfo( 3 )
                                .addKey( new NodeKeyInfo( 45, 45 ) )
                                .addKey( new NodeKeyInfo( 50, 50 ) ),
                        new NodeInfo( 4 )
                                .addKey( new NodeKeyInfo( 70, 70 ) )
                                .addKey( new NodeKeyInfo( 80, 80 ) ),
                        new NodeInfo( 5 )
                                .addKey( new NodeKeyInfo( 100, 100 ) )
                                .addKey( new NodeKeyInfo( 110, 110 ) ),
                        new NodeInfo( 6 )
                                .addKey( new NodeKeyInfo( 130, 130 ) )
                                .addKey( new NodeKeyInfo( 140, 140 ) ),
                        new NodeInfo( 7 )
                                .addKey( new NodeKeyInfo( 160, 160 ) )
                                .addKey( new NodeKeyInfo( 170, 170 ) )
                },
                {
                        new NodeInfo( 1 )
                                .addKey( new NodeKeyInfo( 60, 60 ) )
                                .addKey( new NodeKeyInfo( 90, 90 ) )
                                .addKey( new NodeKeyInfo( 120, 120 ) )
                                .addKey( new NodeKeyInfo( 150, 150 ) )
                                .addChild( 2 )
                                .addChild( 4 )
                                .addChild( 5 )
                                .addChild( 6 )
                                .addChild( 7 ),
                        new NodeInfo( 2 )
                                .addKey( new NodeKeyInfo( 30, 30 ) )
                                .addKey( new NodeKeyInfo( 40, 40 ) )
                                .addKey( new NodeKeyInfo( 45, 45 ) )
                                .addKey( new NodeKeyInfo( 50, 50 ) ),
                        new NodeInfo( 4 )
                                .addKey( new NodeKeyInfo( 70, 70 ) )
                                .addKey( new NodeKeyInfo( 80, 80 ) ),
                        new NodeInfo( 5 )
                                .addKey( new NodeKeyInfo( 100, 100 ) )
                                .addKey( new NodeKeyInfo( 110, 110 ) ),
                        new NodeInfo( 6 )
                                .addKey( new NodeKeyInfo( 130, 130 ) )
                                .addKey( new NodeKeyInfo( 140, 140 ) ),
                        new NodeInfo( 7 )
                                .addKey( new NodeKeyInfo( 160, 160 ) )
                                .addKey( new NodeKeyInfo( 170, 170 ) )
                }
        };

        index = new Index( "TEST_INDEX_BR", 2, Index.KT_LONG, false );
        try
        {
            for( int i = 0; i < inserts.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.insert( new Long( inserts[i][0] ), inserts[i][1] );
                check( resultsInsert[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
        try
        {
            for( int i = 0; i < deletes.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.delete( new Long( deletes[i][0] ), deletes[i][1] );
                check( resultsDelete[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testRemoveBorrowFromLeftRight()
    {
        final int[][] deletes  = {
                { 70, 70 },
                { 60, 60 }
        };
        final NodeInfo[][] resultsDelete = {
                {
                        new NodeInfo( 1 )
                                .addKey( new NodeKeyInfo( 90, 90 ) )
                                .addChild( 8 )
                                .addChild( 9 ),
                        new NodeInfo( 8 )
                                .addKey( new NodeKeyInfo( 30, 30 ) )
                                .addKey( new NodeKeyInfo( 50, 50 ) )
                                .addChild( 2 )
                                .addChild( 3 )
                                .addChild( 4 ),
                        new NodeInfo( 9 )
                                .addKey( new NodeKeyInfo( 120, 120 ) )
                                .addKey( new NodeKeyInfo( 150, 150 ) )
                                .addChild( 5 )
                                .addChild( 6 )
                                .addChild( 7 ),
                        new NodeInfo( 2 )
                                .addKey( new NodeKeyInfo( 10, 10 ) )
                                .addKey( new NodeKeyInfo( 20, 20 ) ),
                        new NodeInfo( 3 )
                                .addKey( new NodeKeyInfo( 40, 40 ) )
                                .addKey( new NodeKeyInfo( 45, 45 ) ),
                        new NodeInfo( 4 )
                                .addKey( new NodeKeyInfo( 60, 60 ) )
                                .addKey( new NodeKeyInfo( 80, 80 ) ),
                        new NodeInfo( 5 )
                                .addKey( new NodeKeyInfo( 100, 100 ) )
                                .addKey( new NodeKeyInfo( 110, 110 ) ),
                        new NodeInfo( 6 )
                                .addKey( new NodeKeyInfo( 130, 130 ) )
                                .addKey( new NodeKeyInfo( 140, 140 ) ),
                        new NodeInfo( 7 )
                                .addKey( new NodeKeyInfo( 160, 160 ) )
                                .addKey( new NodeKeyInfo( 170, 170 ) )
                },
                {
                        new NodeInfo( 1 )
                                .addKey( new NodeKeyInfo( 30, 30 ) )
                                .addKey( new NodeKeyInfo( 90, 90 ) )
                                .addKey( new NodeKeyInfo( 120, 120 ) )
                                .addKey( new NodeKeyInfo( 150, 150 ) )
                                .addChild( 2 )
                                .addChild( 4 )
                                .addChild( 5 )
                                .addChild( 6 )
                                .addChild( 7 ),
                        new NodeInfo( 2 )
                                .addKey( new NodeKeyInfo( 10, 10 ) )
                                .addKey( new NodeKeyInfo( 20, 20 ) ),
                        new NodeInfo( 4 )
                                .addKey( new NodeKeyInfo( 40, 40 ) )
                                .addKey( new NodeKeyInfo( 45, 45 ) )
                                .addKey( new NodeKeyInfo( 50, 50 ) )
                                .addKey( new NodeKeyInfo( 80, 80 ) ),
                        new NodeInfo( 5 )
                                .addKey( new NodeKeyInfo( 100, 100 ) )
                                .addKey( new NodeKeyInfo( 110, 110 ) ),
                        new NodeInfo( 6 )
                                .addKey( new NodeKeyInfo( 130, 130 ) )
                                .addKey( new NodeKeyInfo( 140, 140 ) ),
                        new NodeInfo( 7 )
                                .addKey( new NodeKeyInfo( 160, 160 ) )
                                .addKey( new NodeKeyInfo( 170, 170 ) )
                }
        };

        index = new Index( "TEST_INDEX_BRL", 2, Index.KT_LONG, false );
        try
        {
            for( int i = 0; i < inserts.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.insert( new Long( inserts[i][0] ), inserts[i][1] );
                check( resultsInsert[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
        try
        {
            for( int i = 0; i < deletes.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.delete( new Long( deletes[i][0] ), deletes[i][1] );
                check( resultsDelete[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testMoveToParent()
    {
        final int[][] deletes = {
                { 20, 20 }
        };
        final int[][] inserts = {
                { 10, 10 },
                { 20, 20 },
                { 30, 30 },
                { 40, 40 },
                { 50, 50 }
        };

        final NodeInfo[][] resultsDelete = {
            {
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 10, 10 ) )
                        .addKey( new NodeKeyInfo( 30, 30 ) )
                        .addKey( new NodeKeyInfo( 40, 40 ) )
                        .addKey( new NodeKeyInfo( 50, 50 ) )
            }
        };

        index = new Index( "TEST_INDEX_MP", 2, Index.KT_LONG, false );
        try
        {
            for( int i = 0; i < inserts.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.insert( new Long( inserts[i][0] ), inserts[i][1] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
        try
        {
            for( int i = 0; i < deletes.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.delete( new Long( deletes[i][0] ), deletes[i][1] );
                check( resultsDelete[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testRemoveBorrowFromLeft()
    {
        final int[][] deletes  = {
                { 160, 160 }
        };
        final NodeInfo[][] resultsDelete = {
            {
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 30, 30 ) )
                        .addKey( new NodeKeyInfo( 60, 60 ) )
                        .addKey( new NodeKeyInfo( 90, 90 ) )
                        .addKey( new NodeKeyInfo( 120, 120 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 4 )
                        .addChild( 5 )
                        .addChild( 7 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 10, 10 ) )
                        .addKey( new NodeKeyInfo( 20, 20 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 40, 40 ) )
                        .addKey( new NodeKeyInfo( 45, 45 ) )
                        .addKey( new NodeKeyInfo( 50, 50 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 70, 70 ) )
                        .addKey( new NodeKeyInfo( 80, 80 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 100, 100 ) )
                        .addKey( new NodeKeyInfo( 110, 110 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 130, 130 ) )
                        .addKey( new NodeKeyInfo( 140, 140 ) )
                        .addKey( new NodeKeyInfo( 150, 150 ) )
                        .addKey( new NodeKeyInfo( 170, 170 ) )
            }
        };

        index = new Index( "TEST_INDEX_BL", 2, Index.KT_LONG, false );
        try
        {
            for( int i = 0; i < inserts.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.insert( new Long( inserts[i][0] ), inserts[i][1] );
                check( resultsInsert[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
        try
        {
            for( int i = 0; i < deletes.length; i++ )
            {
//                System.out.println( "Inserting index " + i + " input: " + inputs[i][0] + ", " + inputs[i][1] );
                index.delete( new Long( deletes[i][0] ), deletes[i][1] );
                check( resultsDelete[i] );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testFindFuzzy()
    {
        index = new Index( "TEST_FUZZY", 2, Index.KT_STRING, false );

        try
        {
            index.insert( "calabaza", 1 );
            index.insert( "calabozo", 2 );
            index.insert( "parásito", 3 );
            index.insert( "altamira", 4 );
            index.insert( "altivez", 5 );
            index.insert( "altozano", 6 );
            index.insert( "buenavista", 7 );
            index.insert( "buenos días", 8 );
            index.insert( "añoranza", 9 );
            index.insert( "ñame", 10 );
            index.insert( "políglota", 11 );
            index.insert( "polifacético", 12 );

            int[] results = (int[]) index.findFuzzy( "cala" );
            assertNotNull( results );
            assertEquals( 2, results.length );

            results = (int[]) index.findFuzzy( "calto" );
            assertNull( results );

            results = (int[]) index.findFuzzy( "alt" );
            assertNotNull( results );
            assertEquals( 3, results.length );

            results = (int[]) index.findFuzzy( "polí" );
            assertNotNull( results );
            assertEquals( 1, results.length );

            results = (int[]) index.findFuzzy( "pol" );
            assertNotNull( results );
            assertEquals( 2, results.length );

            results = (int[]) index.findFuzzy( "ñame" );
            assertNotNull( results );
            assertEquals( 1, results.length );
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testFullTextSearch()
    {
        index = new Index( "TEST_FULL", 2, Index.KT_FULL_TEXT, false );

        try
        {
            index.insert( "calabaza", 1 );
            index.insert( "calabozo altamira", 2 );
            index.insert( "parásito", 3 );
            index.insert( "altamira", 4 );
            index.insert( "altivez espartana nómada", 5 );
            index.insert( "altozano", 6 );
            index.insert( "buenavista especial", 7 );
            index.insert( "buenos días", 8 );
            index.insert( "añoranza espacial", 9 );
            index.insert( "ñame parásito", 10 );
            index.insert( "políglota de la calavera", 11 );
            index.insert( "polifacético", 12 );
            index.insert( null, 20 );

            int[] results = (int[]) index.find( "calabaza" );
            assertNotNull( results );
            assertEquals( 1, results.length );

            results = (int[]) index.find( null );
            assertNotNull( results );
            assertEquals( 1, results.length );
            assertEquals( 20, results[0] );

            results = (int[]) index.find( "altama" );
            assertNull( results );

            results = (int[]) index.find( "altamira" );
            assertNotNull( results );
            assertEquals( 2, results.length );

            results = (int[]) index.find( "espartana" );
            assertNotNull( results );
            assertEquals( 1, results.length );

            results = (int[]) index.find( "parásito" );
            assertNotNull( results );
            assertEquals( 2, results.length );

//            results = index.find( "de" );
//            assertNull( results );
//
//            results = index.find( "la" );
//            assertNull( results );

            results = (int[]) index.find( "ñame" );
            assertNotNull( results );
            assertEquals( 1, results.length );

            results = (int[]) index.findFuzzy( "cala" );
            assertNotNull( results );
            assertEquals( 3, results.length );

            results = (int[]) index.findFuzzy( "calto" );
            assertNull( results );

            results = (int[]) index.findFuzzy( "alt" );
            assertNotNull( results );
            assertEquals( 4, results.length );

            results = (int[]) index.findFuzzy( "ñame cala", Index.OR );
            assertNotNull( results );
            assertEquals( 4, results.length );

            results = (int[]) index.findFuzzy( "cala alt", Index.OR );
            assertNotNull( results );
            assertEquals( 6, results.length );

            results = (int[]) index.findFuzzy( "pol cal" );
            assertNotNull( results );
            assertEquals( 1, results.length );

        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testRemove()
    {
        final int[][] inputs = {
            {8,11},
            {16,14},
            {55,7},
            {15,15},
            {8,5},
            {2,13}
        };
        final NodeInfo[][] results = {
        { // scenario 1 (after 8,11)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addChild( 8 )
                        .addChild( 9 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 4 )
                        .addKey( new NodeKeyInfo( 40, 16 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 16, 14 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) ),
                new NodeInfo( 8 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 ),
                new NodeInfo( 9 )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addChild( 6 )
                        .addChild( 4 )
                        .addChild( 7 )
        },
        { // scenario 2 (after 16,14)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addKey( new NodeKeyInfo( 55, 7 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 7 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 40, 16 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
        },
        { // scenario 3 (after 55,7)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addKey( new NodeKeyInfo( 15, 15 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 7 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 40, 16 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
        },
        { // scenario 4 (after 15,15)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 8, 5 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) )
                        .addChild( 2 )
                        .addChild( 5 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 7 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 5 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 40, 16 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
        },
        { // scenario 5 (after 8,5)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 4, 4 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 7 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 2, 13 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 7, 10 ) )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 40, 16 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
        },
        { // scenario 6 (after 2,13)
                new NodeInfo( 1 )
                        .addKey( new NodeKeyInfo( 5, 12 ) )
                        .addKey( new NodeKeyInfo( 23, 6 ) )
                        .addKey( new NodeKeyInfo( 45, 17 ) )
                        .addChild( 2 )
                        .addChild( 3 )
                        .addChild( 6 )
                        .addChild( 7 ),
                new NodeInfo( 2 )
                        .addKey( new NodeKeyInfo( 1, 1 ) )
                        .addKey( new NodeKeyInfo( 4, 4 ) ),
                new NodeInfo( 3 )
                        .addKey( new NodeKeyInfo( 7, 10 ) )
                        .addKey( new NodeKeyInfo( 9, 9 ) )
                        .addKey( new NodeKeyInfo( 10, 2 ) ),
                new NodeInfo( 6 )
                        .addKey( new NodeKeyInfo( 25, 3 ) )
                        .addKey( new NodeKeyInfo( 40, 16 ) ),
                new NodeInfo( 7 )
                        .addKey( new NodeKeyInfo( 75, 18 ) )
                        .addKey( new NodeKeyInfo( 90, 8 ) )
        }
        };
        try
        {
            for( int i = 0; i < inputs.length; i++ )
            {
                System.out.println( "Testing scenario " + (i+1) );
                index.delete( new Long( inputs[i][0] ), inputs[i][1] );
                check( results[i] );
                System.out.println( "Scenario " + (i+1) + " ok" );
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    public void testBulk2()
    {
        profileTest( bulk2, 10 );
    }

    public void testBulk8()
    {
        profileTest( bulk8, 10 );
    }

    public void testBulk50()
    {
        profileTest( bulk50, 10 );
    }

    private void profileTest( final Index bulk, final int iterations )
    {
//        CacheManager.getInstance().setMinFreeMemoryPercent( 60 );
        runsInsert.removeAllElements();
        runsRemove.removeAllElements();
        for(int i = 0; i < iterations; i++)
        {
            System.gc();
            testBulk( bulk2 );
        }
        System.out.println( "Profiling information for " + bulk.getName() );
        long totalRemove = 0;
        long totalInsert = 0;
        long minRemove = Long.MAX_VALUE;
        long minInsert = Long.MAX_VALUE;
        long maxRemove = 0;
        long maxInsert = 0;
        for( int i = 0; i < iterations; i++ )
        {
            final long thisRunInsert = ( (Long) runsInsert.elementAt(i) ).longValue();
            final long thisRunRemove = ( (Long) runsRemove.elementAt(i) ).longValue();
            totalInsert += thisRunInsert;
            totalRemove+= thisRunRemove;
            if( thisRunInsert < minInsert )
            {
                minInsert = thisRunInsert;
            }
            if( thisRunRemove < minRemove )
            {
                minRemove = thisRunRemove;
            }
            if( thisRunInsert > maxInsert )
            {
                maxInsert = thisRunInsert;
            }
            if( thisRunRemove > maxRemove )
            {
                maxRemove = thisRunRemove;
            }
        }
        System.out.println( "************************************************" );
        System.out.println( "Total insert time: " + totalInsert + "ms" );
        System.out.println( "Min insert time: " + minInsert + "ms" );
        System.out.println( "Max insert time: " + maxInsert + "ms" );
        System.out.println( "------------------------------------------------" );
        System.out.println( "Total remove time: " + totalRemove  + "ms" );
        System.out.println( "Min remove time: " + minRemove + "ms" );
        System.out.println( "Max remove time: " + maxRemove + "ms" );
        System.out.println();
    }

    private void testBulk( final Index bulk )
    {
        try
        {
//            final long[] bulkTestKeys = new long[1000];
//            final Random random = new Random( System.currentTimeMillis() );
//            for( int i = 0; i < 1000; i++ )
//            {
//                bulkTestKeys[i] = random.nextLong() % 3000;
//            }
//            System.out.print( "long[] bulkTestKeys = {" );
//            for( int i = 0; i < 1000; i++ )
//            {
//                System.out.print( " " + bulkTestKeys[i] + "L" );
//                if( i < 999 )
//                {
//                    System.out.print( "," );
//                }
//                if( i % 3 == 0 )
//                {
//                    System.out.println();
//                }
//            }
//            System.out.println( " };" );

            long start = System.currentTimeMillis();
            int count = 0;
            for( int i = 0; i < 1000; i++ )
            {
                final int result = bulk.insert( new Long( BULK_TEST_KEYS[i] ), i );
//                System.out.println( "Inserted node " + i + ", key: " + bulkTestKeys[i] + ", result: " + result );
                if( result == 2 )
                {
                    count++;
                }
                assertEquals( count, bulk.size() );
            }
            long end = System.currentTimeMillis();
//            System.out.println( "Unique bulkTestKeys: " + count + " inserted in " + (end - start) + "ms, " + ((end - start)/1000) + "s, " + ((end - start)/1000000) + "avg s" );
            runsInsert.addElement( new Long( end - start ) );
            linkCheck( bulk.getRootNode() );
//            testSpeed( index );
//            printTree( bulk.getRootNode(), "" );
            start = System.currentTimeMillis();
            for( int i = 0; i < 1000; i++ )
            {
                bulk.delete( new Long( BULK_TEST_KEYS[i] ), i );
                linkCheck( bulk.getRootNode() );
//                System.out.println( "\nAfter removing node " + i + " with key: " + bulkTestKeys[i] +"\n" );
//                printTree( bulk.getRootNode(), "" );
            }
            end = System.currentTimeMillis();
//            System.out.println( "Unique bulkTestKeys: " + count + " removed in " + (end - start) + "ms, " + ((end - start)/1000) + "s, " + ((end - start)/1000000) + "avg s" );
            runsRemove.addElement( new Long( end - start ) );
        }
        catch( Exception e )
        {
            e.printStackTrace();
            try
            {
                printTree( bulk.getRootNode(), "" );
            }
            catch( Exception e1 )
            {
                e1.printStackTrace();
            }
            fail( e.getMessage() );
        }
    }

    private void linkCheck( final Node node )
    {
        try
        {
            if( !node.isLeaf() )
            {
                for( int i = 0; i <= node.getKeyCount(); i++ )
                {
                    final Node child = node.getChildNode( i );
                    if( child.getParent() != node )
                    {
                        fail( "Nodo hijo no coincide con padre: child: " + child + ", parent: " + node + ", padre apuntado: " + child.getParent() );
                    }
                    if( !child.isLeaf() )
                    {
                        linkCheck( child );
                    }
                }
            }
        }
        catch( DBException e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
    }

    private void printTree( final Node node, final String prefix )
            throws DBException
    {
        System.out.print( prefix + "{ [" + node.getRecordId() + "]: " );
        for( int i = 0; i < node.getKeyCount(); i++ )
        {
            System.out.print( "{" + node.getKey( i ).getKey() + "}" );
            if( i < (node.getKeyCount() -1) )
            {
                System.out.print( ", " );
            }
        }
        System.out.println( "}" );
        if( !node.isLeaf() )
        {
            for( int i = 0; i <= node.getKeyCount(); i++ )
            {
                if( node.getChild( i ) == 0 )
                {
                    System.out.println( prefix + "  NULL CHILD at " + i );
                }
                else
                {
                    printTree(
                            node.getIndex().getNode( node, node.getChild( i ) ),
                            prefix + "  "
                    );
                }
            }
        }
    }

//    private void testSpeed( final Index index )
//    {
//        try
//        {
//            System.out.println( "Speed test for " + index.getName() );
//            long start = System.currentTimeMillis();
//            for( int i = 0; i < 100; i++ )
//            {
//                bulk2.find( new Long( BULK_TEST_KEYS[100 + i] ) );
//            }
//            long end = System.currentTimeMillis();
//            System.out.println( "Searched 100 keys in " + (end - start) + "ms" );
//        }
//        catch( Exception e )
//        {
//            e.printStackTrace();
//            fail( e.getMessage() );
//        }
//    }

    public void testNullKey()
    {
        Index index = new Index( "NULL_INDEX", 2, Index.KT_LONG, false );
        try
        {
            index.insert( null, 1 );
            index.insert( new Integer( 100 ), 2 );
            index.insert( null, 3 );

            final int[] result = (int[]) index.find( null );
            assertNotNull( result );
            assertEquals( 2, result.length );
            assertEquals( 1, result[0] );
            assertEquals( 3, result[1] );
        }
        catch( Exception e )
        {
            fail( e.getMessage() );
        }
    }

    public void testFullTextSearchFromResource()
    {
        InputStream is = null;
        Index index = new Index( "FTS_RES", 50, Index.KT_FULL_TEXT, false );
        try
        {
            is = getClass().getResourceAsStream( "/clientes.csv" );
            StringBuffer buffer = new StringBuffer();
            int c = is.read();
            int recordId = 1;
            while( c != -1 )
            {
                if( c == '\n' )
                {
                    index.insert( buffer.toString(), recordId++ );
                    buffer = new StringBuffer();
                }
                else
                {
                    buffer.append( (char) c );
                }
                c = is.read();
            }
            if( buffer.length() > 0 )
            {
                index.insert( buffer.toString(), recordId++ );
            }

            final int[] op = new int[]{ 35, 37, 170, 347, 348, 1273, 1409, 1484, 1485, 1769 };
            final int[] result = (int[]) index.findFuzzy( "ana" );
            assertNotNull( result );
            assertEquals( op.length, result.length );
            for( int i = 0; i < op.length; i++ )
            {
                boolean found = false;
                for( int j = 0; j < result.length && !found; j++ )
                {
                    found |= (op[i] == result[j]);
                }
                if( !found )
                {
                    fail( "No match! " + op + found);
                }
            }
        }
        catch( Exception e )
        {
            e.printStackTrace();
            fail( e.getMessage() );
        }
        finally
        {
            if( is != null ) try{ is.close(); }catch( Exception e ){}
        }
    }

    public static TestSuite suite()
    {
        //noinspection InstantiatingObjectToGetClassObject
        final TestSuite suite = new TestSuite(
                new IndexTest().getClass().getName()
        );

        try
        {
            suite.addTest(new IndexTest("testNullKey"));
            suite.addTest(new IndexTest("testInsert"));
            suite.addTest(new IndexTest("testFind"));
            suite.addTest(new IndexTest("testRemove"));
            suite.addTest(new IndexTest("testRemoveBorrowFromRight"));
            suite.addTest(new IndexTest("testRemoveBorrowFromLeftRight"));
            suite.addTest(new IndexTest("testRemoveBorrowFromLeft"));
            suite.addTest(new IndexTest("testMoveToParent"));
            suite.addTest(new IndexTest("testFindFuzzy"));
            suite.addTest(new IndexTest("testFullTextSearch"));
            suite.addTest(new IndexTest("testFullTextSearchFromResource"));

            suite.addTest(new IndexTest("testBulk2"));
            suite.addTest(new IndexTest("testBulk8"));
            suite.addTest(new IndexTest("testBulk50"));
        }
        catch( Exception e )
        {
            fail(e.getMessage());
        }

        return suite;
    }

    private class NodeInfo
    {
        int      recordId;
        Vector   keyInfo = new Vector( 1 );
        Vector   children = new Vector( 1 );

        public NodeInfo( final int recordId )
        {
            this.recordId = recordId;
        }

        public NodeInfo addKey( final NodeKeyInfo keyInfo )
        {
            this.keyInfo.addElement( keyInfo );
            return this;
        }

        public NodeInfo addChild( final int child )
        {
            children.addElement( new Integer( child ) );
            return this;
        }
    }

    private class NodeKeyInfo
    {
        long        key;
        Vector      values = new Vector( 1 );

        public NodeKeyInfo( final long key, final int value )
        {
            this.key = key;
            values.addElement( new Integer( value ) );
        }

        public NodeKeyInfo addValue( final int value )
        {
            values.addElement( new Integer( value ) );
            return this;
        }
    }
}
