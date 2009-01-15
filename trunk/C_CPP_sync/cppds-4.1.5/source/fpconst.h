//=========================================================
//  Numerics Library
//=========================================================
//  FPConst.h
//
//  Definitions of various mathematical constants. These
//  constants assume that float is an IEEE single, that
//  double is an IEEE double, and that long double is
//  the 80-bit extended IEEE double used as a temporary
//  type by Intel 80x87 math coprocessors.
//
//  Copyright 1991-1996 Scott Robert Ladd.
//=========================================================

#ifndef FPCONST_H
#define FPCONST_H

//----------------
// float constants
//----------------
const float N_FPI         =  3.141593F;     // pi
const float N_F1PI        =  0.318310F;     // 1 / pi
const float N_F2PI        =  0.636620F;     // 2 / pi
const float N_FPISQR      =  9.869604F;     // pi squared
const float N_FPISQRT     =  1.772454F;     // square root pi
const float N_FPI2        =  1.570796F;     // pi / 2
const float N_FPI3        =  1.047198F;     // pi / 3
const float N_FPI4        =  0.785398F;     // pi / 4
const float N_FPILOG      =  1.144730F;     // log(pi)
const float N_FPILOG10    =  0.497150F;     // log10(pi)

const float N_FE          =  2.718282F;     // e
const float N_F1E         =  0.367879F;     // 1 / e
const float N_FESQR       =  7.389056F;     // e squared
const float N_FLOG10      =  2.302585F;     // log(10)
const float N_FLOG2       =  0.693147F;     // log(2)
const float N_FLOG10E     =  0.434294F;     // log10(e)
const float N_FLOG10SQE   =  0.217147F;     // log10(sqrt(e))
const float N_FLOG102     =  0.301030F;     // log10(2)

const float N_FSQRT2      =  1.414214F;     // sqrt(2)
const float N_FEULER      =  0.577216F;     // Euler's number
const float N_FEULORLOG   = -0.549539F;     // log(Euler)
const float N_FGOLDRATIO  =  1.618034F;     // Golden ratio

const float N_FDEGPERRAD  = 57.2958F;       // 180 / pi
const float N_FRADPERDEG  =  0.0174533F;    // pi / 180

// astronomy
const float NF_c             = 299792500.0F;  // m/sec
const float NF_SiderealYear  = 365.2564F;     // days
const float NF_TropicalYear  = 365.2422F;
const float NF_JulianYear    = 365.25F;
const float NF_AnomalYear    = 365.2596F;
const float NF_SiderealRatio = 1.002738F;
const float NF_SynodicMonth  = 29.5306F;      // days
const float NF_SiderealMonth = 27.3217F;
const float NF_AnomalMonth   = 27.5546F;
const float NF_DraconMonth   = 27.2122F;
const float NF_SolarDay      = 24.06571F;     // hours
const float NF_SiderealDay   = 23.93447F;
const float NF_RotationDay   = 23.93447F;
const float NF_EarthMass     = 5.974E+24F;    // kg
const float NF_EarthRadius   = 6378140.0F;    // m
const float NF_EarthRadiusP  = 6356755.0F;    // m
const float NF_EarthG        = 9.80665F;      // m/s^2
const float NF_AU            = 149597900000.0F; // m
const float NF_LightYear     = 9.46073E+15F;  // m
const float NF_Atmosphere    = 9.80665E+04F;  // Pa
const float NF_TechAtmosph   = 1.01325E+05F;  // Pa

// conversions
const float NF_cmPerInch     = 2.54F;
const float NF_mPerFoot      = 3.048F;
const float NF_mPerMile      = 1609.344F;
const float NF_mPerNautMile  = 1852.0F;
const float NF_mPerYard      = 0.9144F;
const float NF_kgPerPound    = 0.4535924F;
const float NF_kgPerShortTon = 907.18474F;
const float NF_kgPerLongTon  = 1016.047F;

// physics
const float NF_ElecCharge   = 1.602177E-19F; // C
const float NF_ElecMass     = 9.109390E-31F; // kg
const float NF_ElecRadius   = 2.817941E-15F; // m
const float NF_ElecMoment   = 9.284770E-24F; // J/T
const float NF_ElecAngular  = 5.2729E-35F;   // J * sec
const float NF_ProtMass     = 1.672623E-27F; // kg
const float NF_ProtMoment   = 1.410608E-26F; // J/T
const float NF_NeutMass     = 1.674929E-27F; // kg
const float NF_AMU          = 1.660540E-27F; // kg
const float NF_Permittivity = 8.854188E-12F; // F/m
const float NF_Permeability = 1.256637E-06F; // H/m
const float NF_Avagadro     = 6.022137E+23F; // 1/mol
const float NF_Boltzmann    = 1.380657E-23F; // J/K
const float NF_Planck       = 6.626075E-34F; // J * sec
const float NF_PlanckBar    = 1.054573E-34F; // h/2pi
const float NF_UnivGas      = 8.314510F;     // J/mol * K
const float NF_MolarVolume = 0.02241410F;   // m^3/mol
const float NF_Faraday   = 9.648531E+04F; // C/mol
const float NF_StefBoltz = 5.67050E-08F;  // W/(m^2 * K^4)
const float NF_Rydberg   = 1.097373E+07F; // 1/m
const float NF_G         = 6.67260E-11F;  // (m^3)/(s^2*kg)
const float NF_ComptonWave = 2.426311E-12F; // m
const float NF_BohrRadius  = 5.291772E-11F; // m
const float NF_BohrMagneton = 9.274015E-24F; // J/T

//-----------------
// double constants
//-----------------
const double N_DPI         =  3.141592653589793238;
const double N_D1PI        =  0.3183098861837907;
const double N_D2PI        =  0.6366197723675813;
const double N_DPISQR      =  9.869604401089359;
const double N_DPISQRT     =  1.77245385090552;
const double N_DPI2        =  1.570796326794897;
const double N_DPI3        =  1.04719755119660;
const double N_DPI4        =  0.7853981633974483;
const double N_DPILOG      =  1.144729885849400;
const double N_DPILOG10    =  0.497149872694134;

const double N_DE          =  2.718281828459045;
const double N_D1E         =  0.367879441171442;
const double N_DESQR       =  7.389056098930650;
const double N_DLOG10      =  2.302585092994046;
const double N_DLOG2       =  0.6931471805599453;
const double N_DLOG10E     =  0.4342944819032518;
const double N_DLOG10SQE   =  0.2171472409516259;
const double N_DLOG102     =  0.301029995663981;

const double N_DSQRT2      =  1.414213562373095;
const double N_DEULER      =  0.5772156649015329;
const double N_DEULORLOG   = -0.5495393129816448;
const double N_DGOLDRATIO  =  1.618033988749895;

const double N_DDEGPERRAD  = 57.2957795130823;
const double N_DRADPERDEG  =  0.0174532925199433;

// astronomy
const double ND_c             = 299792458.0;
const double ND_SiderealYear  = 365.2564;   
const double ND_TropicalYear  = 365.2422;
const double ND_JulianYear    = 365.25;
const double ND_AnomalYear    = 365.2596;
const double ND_SiderealRatio = 1.0027379092558;
const double ND_SynodicMonth  = 29.5306;    
const double ND_SiderealMonth = 27.3217;
const double ND_AnomalMonth   = 27.5546;
const double ND_DraconMonth   = 27.2122;
const double ND_SolarDay      = 24.06571111;
const double ND_SiderealDay   = 23.93446944;
const double ND_RotationDay   = 23.93447222;
const double ND_EarthMass     = 5.974E+24;   
const double ND_EarthRadius   = 6378140.0;   
const double ND_EarthRadiusP  = 6356755.0;   
const double ND_EarthG        = 9.80665;     
const double ND_AU            = 149597870000.0;
const double ND_LightYear     = 9.46073E+15; 
const double ND_Atmosphere    = 9.80665E+04; 
const double ND_TechAtmosph   = 1.01325E+05; 

// conversions
const double ND_cmPerInch     = 2.54;
const double ND_mPerFoot      = 3.048;
const double ND_mPerMile      = 1609.344;
const double ND_mPerNautMile  = 1852.0;
const double ND_mPerYard      = 0.9144;
const double ND_kgPerPound    = 0.45359237;
const double ND_kgPerShortTon = 907.18474;
const double ND_kgPerLongTon  = 1016.0469088;

// physics
const double ND_ElecCharge   = 1.60217738E-19;   
const double ND_ElecMass     = 9.1093897E-31;    
const double ND_ElecRadius   = 2.81794092E-15;   
const double ND_ElecMoment   = 9.2847700E-24;    
const double ND_ElecAngular  = 5.2729E-35;       
const double ND_ProtMass     = 1.6726230E-27;    
const double ND_ProtMoment   = 1.41060761E-26;   
const double ND_NeutMass     = 1.6749286E-27;    
const double ND_AMU          = 1.6605402E-27;    
const double ND_Permittivity = 8.85418781762E-12;
const double ND_Permeability = 1.25663706143E-06;
const double ND_Avagadro     = 6.0221367E+23;    
const double ND_Boltzmann    = 1.380657E-23;     
const double ND_Planck       = 6.6260754E-34;    
const double ND_PlanckBar    = 1.05457266E-34;   
const double ND_UnivGas      = 8.314510;         
const double ND_MolarVolume  = 0.02241410;       
const double ND_Faraday      = 9.6485309E+04;    
const double ND_StefBoltz    = 5.67050E-08;      
const double ND_Rydberg      = 1.0973731534E+07; 
const double ND_G            = 6.67260E-11;      
const double ND_ComptonWave  = 2.42631058E-12;   
const double ND_BohrRadius   = 5.29177249E-11;   
const double ND_BohrMagneton = 9.2740154E-24;    

//----------------------
// long double constants
//----------------------
const long double N_EPI         =  3.141592653589793238L;
const long double N_E1PI        =  0.3183098861837906715L;
const long double N_E2PI        =  0.6366197723675813431L;
const long double N_EPISQR      =  9.869604401089358619L;
const long double N_EPISQRT     =  1.77245385090551603L;
const long double N_EPI2        =  1.570796326794896619L;
const long double N_EPI3        =  1.04719755119659775L;
const long double N_EPI4        =  0.7853981633974483097L;
const long double N_EPILOG      =  1.144729885849400174L;
const long double N_EPILOG10    =  0.497149872694133854L;

const long double N_EE          =  2.718281828459045235L;
const long double N_E1E         =  0.367879441171442322L;
const long double N_EESQR       =  7.389056098930650227L;
const long double N_ELOG10      =  2.302585092994045684L;
const long double N_ELOG2       =  0.6931471805599453094L;
const long double N_ELOG10E     =  0.4342944819032518277L;
const long double N_ELOG10SQE   =  0.217147240951625914L;
const long double N_ELOG102     =  0.301029995663981195L;

const long double N_ESQRT2      =  1.414213562373095049L;
const long double N_EEULER      =  0.5772156649015328606L;
const long double N_EEULORLOG   = -0.5495393129816448223L;
const long double N_EGOLDRATIO  =  1.618033988749894848L;

const long double N_EDEGPERRAD  = 57.2957795130823209L;
const long double N_ERADPERDEG  =  0.0174532925199432957L;

// astronomy
const long double NL_c             = 299792458.0L; 
const long double NL_SiderealYear  = 365.2564L;    
const long double NL_TropicalYear  = 365.2422L;
const long double NL_JulianYear    = 365.25L;
const long double NL_AnomalYear    = 365.2596L;
const long double NL_SiderealRatio = 1.0027379092558L;
const long double NL_SynodicMonth  = 29.5306L;     
const long double NL_SiderealMonth = 27.3217L;
const long double NL_AnomalMonth   = 27.5546L;
const long double NL_DraconMonth   = 27.2122L;
const long double NL_SolarDay      = 24.06571111L; 
const long double NL_SiderealDay   = 23.93446944L;
const long double NL_RotationDay   = 23.93447222L;
const long double NL_EarthMass     = 5.974E+24L;   
const long double NL_EarthRadius   = 6378140.0L;   
const long double NL_EarthRadiusP  = 6356755.0L;   
const long double NL_EarthG        = 9.80665L;     
const long double NL_AU            = 149597870000.0L;
const long double NL_LightYear     = 9.46073E+15L; 
const long double NL_Atmosphere    = 9.80665E+04L; 
const long double NL_TechAtomos    = 1.01325E+05L; 

// conversions
const long double NL_cmPerInch     = 2.54L;
const long double NL_mPerFoot      = 3.048L;
const long double NL_mPerMile      = 1609.344L;
const long double NL_mPerNautMile  = 1852.0L;
const long double NL_mPerYard      = 0.9144L;
const long double NL_kgPerPound    = 0.45359237L;
const long double NL_kgPerShortTon = 907.18474L;
const long double NL_kgPerLongTon  = 1016.0469088L;

// physics
const long double NL_ElecCharge   = 1.60217738E-19L;   
const long double NL_ElecMass     = 9.1093897E-31L;    
const long double NL_ElecRadius   = 2.81794092E-15L;   
const long double NL_ElecMoment   = 9.2847700E-24L;    
const long double NL_ElecAngular  = 5.2729E-35L;       
const long double NL_ProtMass     = 1.6726230E-27L;    
const long double NL_ProtMoment   = 1.41060761E-26L;   
const long double NL_NeutMass     = 1.6749286E-27L;    
const long double NL_AMU          = 1.6605402E-27L;    
const long double NL_Permittivity = 8.85418781762E-12L;
const long double NL_Permeability = 1.25663706143E-06L;
const long double NL_Avagadro     = 6.0221367E+23L;    
const long double NL_Boltzmann    = 1.380657E-23L;     
const long double NL_Planck       = 6.6260754E-34L;    
const long double NL_PlanckBar    = 1.05457266E-34L;   
const long double NL_UnivGas      = 8.314510L;         
const long double NL_MolarVolume  = 0.02241410L;       
const long double NL_Faraday      = 9.6485309E+04L;    
const long double NL_StefBoltz    = 5.67050E-08L;      
const long double NL_Rydberg      = 1.0973731534E+07L; 
const long double NL_G            = 6.67260E-11L;      
const long double NL_ComptonWave  = 2.42631058E-12L;   
const long double NL_BohrRadius   = 5.29177249E-11L;   
const long double NL_BohrMagneton = 9.2740154E-24L;

#endif




