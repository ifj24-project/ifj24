1) <start> -> <program_prolog><program>
2) <program_prolog> -> const "id" = @import("ifj24.zig");
3) <program> -> <func_define><program>
4) <program> -> ε

5) <data_type> -> i32
6) <data_type> -> f64
7) <data_type> -> []u8
8) <data_type> -> void

9) <func_define> -> pub fn "id"(<params_define>) <data_type>{<func_body>}
10) <params_define> -> ε
11) <params_define> -> "id": <data_type><params_define_next>
12) <params_define_next> -> ε
13) <params_define_next> -> ,"id": <data_type><params_define_next>
14) <func_body> ->ε
15) <func_body> -> <statement><func_body>

16) <statement> -> <variable_define>
17) <statement> -> <variable_assign>
18) <statement> -> <if>
19) <statement> -> <while>
20) <statement> -> <void_call>
21) <statement> -> <return_statement>

22) <variable_define> -> const "id" : <data_type> = <EXPRESSION>;
23) <variable_define> -> const "id" : <data_type> = <func_call>;
24) <variable_define> -> const "id" : <data_type> = "string";
25) <variable_define> -> const "id" : <data_type> = "id";
26) <variable_define> -> var "id" : <data_type> = <EXPRESSION>;
27) <variable_define> -> var "id" : <data_type> = <func_call>;
28) <variable_define> -> var "id" : <data_type> = "string";
29) <variable_define> -> var "id" : <data_type> = "id";
30) <variable_assign> -> "id" = <EXPRESSION>;
31) <variable_assign> -> "id" = <func_call>;
32) <variable_assign> -> "id"= "string";
33) <func_call> -> "id"(<params>)
34) <params> -> ε
35) <params> -> "string"<params_next>
36) <params> -> "id"<params_next>
37) <params> -> <EXPRESSION><params_next>
38) <params> -> <func_call><params_next>
39) <params_next> ->  ε
40) <params_next> ->, "string"<params_next>
41) <params_next> ->, "id"<params_next>
42) <params_next> -> ,<EXPRESSION><params_next>
43) <params_next> -> ,<func_call><params_next>
44) <if> -> if (<EXPRESSION>){<func_body>}else{<func_body>} 
45) <if> -> if (<EXPRESSION>) |"id"| {<func_body>}else{<func_body>} 
46) <while> ->while (<EXPRESSION>){<func_body>}
47) <while> ->while (<EXPRESSION>) |"id"| {<func_body>}
48) <void_call> -> "id"(<params>);
49) <return_statement> -> return;
50) <return_statement> -> return "string";
51) <return_statement> -> return "id";
52) <return_statement> -> return <EXPRESSION>;

53) <EXPRESSION> -> <EXPRESSION> > <compared>
54) <EXPRESSION>-> <EXPRESSION> >= <compared>
55) <EXPRESSION>-> <EXPRESSION> < <compared>
56) <EXPRESSION>-> <EXPRESSION> <= <compared>
57) <EXPRESSION>-> <EXPRESSION> == <compared>
58) <EXPRESSION>-> <EXPRESSION> != <compared>
59) <EXPRESSION>-><compared>
60) <compared> -> <compared> + <term>
61) <compared> -> <compared> - <term>
62) <compared> -> <term>
63) <term> -> <term> * <factor>
64) <term> -> <term> / <factor>
65) <term> -> <factor>
66) <factor> -> "id"
67) <factor> -> "int"
68) <factor> -> "float"
69) <factor> -> (<EXPRESSION>)