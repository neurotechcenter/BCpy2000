        €€  €€                  …   0   R E G I S T R Y   €€       0 	        HKCR
{
	NoRemove AppID
	{
		'{CBF4944A-318B-4447-BF30-F274C1CD69C9}' = s 'BCI2000Automation'
		'BCI2000Automation.DLL'
		{
			val AppID = s '{CBF4944A-318B-4447-BF30-F274C1CD69C9}'
		}
	}
}
   Ќ  0   R E G I S T R Y   €€       0 	        HKCR
{
	BCI2000Automation.BCI2000Remote.1 = s 'BCI2000Remote Class'
	{
		CLSID = s '{F880B1BC-3511-4b06-8A79-895477300608}'
	}
	BCI2000Automation.BCI2000Remote = s 'BCI2000Remote Class'
	{
		CLSID = s '{F880B1BC-3511-4b06-8A79-895477300608}'
		CurVer = s 'BCI2000Automation.BCI2000Remote.1'
	}
	NoRemove CLSID
	{
		ForceRemove {F880B1BC-3511-4b06-8A79-895477300608} = s 'BCI2000Remote Class'
		{
			ProgID = s 'BCI2000Automation.BCI2000Remote.1'
			VersionIndependentProgID = s 'BCI2000Automation.BCI2000Remote'
			ForceRemove 'Programmable'
			InprocServer32 = s '%MODULE%'
			{
				val ThreadingModel = s 'Apartment'
			}
			'TypeLib' = s '{363D672D-41EC-4b3e-AE58-50B49A83D530}'
		}
	}
}
   ђ  ,   T Y P E L I B   €€     0 	        MSFT      	      A                        6         €€€€       А             d   L  »   €€€€   d     €€€€   p     €€€€   T     €€€€     А   €€€€   Ф  ј   €€€€   М     €€€€   М  а  €€€€   l
  А  €€€€   м  8   €€€€   €€€€    €€€€   $  T   €€€€   x  $   €€€€   €€€€    €€€€   €€€€    €€€€   %"  Ь                                     `              $           €€€€                  €€€€4" Ь                 $                   ®   @  <       <           €€€€ ђ             €€€€€€€€€€€€Р   €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€®       €€€€€€€€€€€€€€€€H   `   €€€€€€€€€€€€€€€€€€€€0   -g=6мA>KЃXPіЪГ’0ю€€€€€€€eЇwё|Q—ҐЏ  шw<й€€€€€€€€cЇwё|Q—ҐЏ  шw<й€€€€€€€€dЇwё|Q—ҐЏ  шw<й€€€€€€€€Љ±Аш5KКyЙTw0       0     ј      F   €€€€      ј      F   €€€€lџ•“[ёJ†B?LпЄПOd   x   d      €€€€€€€€      Р   x          - stdole2.tlbWWW€€€€€€€€€€€€€€€€  €€€€А   €€€€€€€€€€€€ь  €€€€H  ‘  €€€€€€€€€€€€€€€€€€€€€€€€а  €€€€€€€€€€€€X  €€€€і  €€€€€€€€м  €€€€®  €€€€€€€€€€€€€€€€€€€€x      ƒ   \  €€€€€€€€€€€€€€€€€€€€X   €€€€Ў   €€€€  €€€€€€€€0    €€€€€€€€€€€€€€€€€€€€€€€€€€€€4    €€€€€€€€€€€€€€€€Д  €€€€  €€€€€€€€€€€€€€€€Ф  €€€€€€€€€€€€€€€€€€€€€€€€Ь  »  \  `  €€€€€€€€l   H  €€€€ф  ђ  €€€€0  €€€€€€€€€€€€€€€€€€€€–  €€€€€€€€ƒ  €€€€€€€€€€€€€€€€И  Ф  €€€€p  ј  €€€€     ®   4  H  €€€€€€€€∞  x  €€€€€€€€€€€€€€€€€€€€€€€€€€€€ оBCI2000AutomationLib    €€€€8¶ BCI2000RemoteWWWd   €€€€8№фIBCI2000RemoteWWd   €€€€ ЃTimeoutW€€€€€€€€ ЎГsecondsWd   €€€€ КOperatorPath€€€€€€€€ 2pathd   €€€€ tМTelnetAddressWWW€€€€€€€€ І≈addressWd   €€€€ 0ЪWindowVisibleWWW€€€€€€€€ ґ”visibleWd   ф    6fWindowTitleW€€€€€€€€ т~titleWWWd   €€€€ urResultWWd   €€€€	 МЏSubjectIDWWWd   €€€€	 ’;SessionIDWWWd   <    \ТDataDirectoryWWW€€€€€€€€	 mGdirectoryWWWd   x   \…ConnectW€€€€€€€€ ё|successWd   €€€€
 RDisconnectWWd   €€€€ StartupModulesWW€€€€     ДmodulesWd   €€€€	 sУSetConfigWWWd   €€€€ ЊStartWWWd   €€€€ ц4Stopd   €€€€ /LoadParametersLocalW€€€€€€€€ %рfiled   €€€€ мhLoadParametersRemoted   €€€€ сGetParameter€€€€€€€€ ртname€€€€€€€€ дKvalueWWWd   €€€€ (¶SetParameterd   €€€€ КсAddStateVariable€€€€Ш   	 2ЮstateNameWWW€€€€ј   ^[bitWidth€€€€€€€€ [ДinitialValued   д   (GetStateVariabled   €€€€ zSetStateVariabled   €€€€ K>GetSystemStateWW€€€€€€€€ щstateWWWd   €€€€ g
GetControlSignal€€€€€€€€ тchannelW€€€€D   [elementWd   €€€€ ∆рEncodeWW€€€€€€€€ ?ГrawValue€€€€€€€€ 5ЉencodedValued   €€€€ YЌExecuteW€€€€€€€€ ‘]commandW€€€€€€€€ o{exitCoded   €€€€	 DOSetScriptWWW€€€€€€€€	 “teventNameWWW€€€€€€€€ `scriptWWd   €€€€	 ”фGetScriptWWW" BCI2000Automation 1.1 Type Library BCI2000Remote ClassWWW# BCI2000 Remote controller interfaceWWW Timeout in seconds.WWW5 Path to operator module, or batch file. May be empty.WA Telnet address to use for connection. Defaults to localhost:3999.W4 Sets and gets visibility of the BCI2000 main window.WW3 Sets and gets the title of the BCI2000 main window.WWWA Text output of the previously executed command, or error message.W4 Sets and gets the subject ID for the next recording.WW4 Sets and gets the session ID for the next recording.WW8 Sets and gets the data directory for the next recording.WWi Connect to BCI2000. When OperatorPath is not empty, this starts up the operator module before connecting.WА Disconnect from an existing connection. Terminates the running operator module if it was started by the previous Connect() call.WWa Start BCI2000 core modules, listed by executable name, including possible command line arguments.WK Apply current set of parameters. Will be called from Start() automatically.WWW5 Starts a new run (recording) with current parameters.W" Stops the current run (recording).G Load parameters from a file, relative to the current working directory.WWWE Load parameters from a file, relative to BCI2000's working directory.W& Gets the value of a BCI2000 parameter.& Sets the value of a BCI2000 parameter.% Creates a new BCI2000 state variable.W+ Gets the value of a BCI2000 state variable.WWW+ Sets the value of a BCI2000 state variable.WWW3 Gets the current system state (state of operation).WWW- Gets the value of the BCI2000 control signal.Wв Encode a string value for use as a single argument, or in parameter lines, when calling Execute(). In the resulting string, white space and special characters will be encoded as hexadecimal values with a % character prepended.
Execute a BCI2000 scripting command. If the command executed a shell command, returns the command's exit code. Otherwise, the return value indicates interpretation of the command's result as a truth value: 0 for "true", 1 for "false", and -1 if there is no obv?µ??', Associates scripting commands with an event.WW1 Gets scripting commands associated with an event.W @ А @ А @ А @ А   А `     @ А >   Created by MIDL version 7.00.0555 at Wed Mar 20 15:00:21 2013
 чјIQWW + WW       €€€€0   D       H   L      \  ,    А     L D        d       l   
   ,   А      D !         d    А€€€€   ,   А    $ L D        |      Ш   
   ,   А    ( D !        |    А€€€€   ,   А    , L D        і      ƒ   
   ,   А    0 D !        і    А€€€€   ,   А    4 L D        ш      ф   
   ,   А    8 D !        ш    А€€€€   ,   А    < L D	        0        
   , 	  А    @ D !        0   А€€€€   , 
  А    D L D
        h     4  
   ,   А    H L D        ђ     H  
   ,   А    L D !        ђ   А€€€€   ,   А    P L D        д     `  
   ,   А    T D !        д   А€€€€   ,   А    X L D             Ф  
   ,   А    \ D !           А€€€€   ,   А    ` L 	D        X     ј  
   ,   А    d L 	D        ƒ     ј  
   8   А    h l 	D        H  (           ј  
   ,   А    l L 	D        ђ     ј  
   ,   А    p L 	D        ь     ј  
   ,   А    t L 	D        4     ј  
   8   А    x \ 	D        X   Аx        ј  
   8   А    | \ 	D        §   Аx        ј  
   D   А    А t 	D        м   Ај        –        ј  
   D   А    Д l 	D           Ај      А–        ј  
   P   А    И | 	D        <   А      А0      АD        ј  
   D   А    М t 	D        d   А         –        ј  
   D   А    Р l 	D        Ф   А      А–        ј  
   8   А    Ф d 	D        ƒ     ∞        ј  
   P   А    Ш Д 	D        ь   Аа      Аф         –        ј  
   8    А    Ь \ 	D         ,   А        0  
   D !  А    † t 	D!          А\     0   p        ј  
   D "  А    § l 	D"           АЬ      Аі        ј  
   D #  А    ® t 	D#        L   АЬ        і        ј  
     `  ` ` ` ` ` ` ` ` `
 ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` `  `! `" `# `X   X   А   А   ®   ®   Ў   Ў       4  H  H  `  `  x  x  ђ  ‘  м    4  H  X  И  ®  д  ь  \  x  Ф  ƒ    H  Д  »      ,   X   Д   ∞   №     4  `  М  Є  д    <  h  Ф  ј  м    D  |  ®  ‘     8  p  і  ш  H  М  –    X  Р  ‘    8       €€ €€     0	                   O p e r a t o r . e x e                     