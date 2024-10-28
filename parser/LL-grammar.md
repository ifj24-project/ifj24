## LL-grammar

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
24) <variable_define> -> var "id" : <data_type> = <EXPRESSION>;
25) <variable_define> -> var "id" : <data_type> = <func_call>;
26) <variable_assign> -> "id" = <EXPRESSION>;
27) <variable_assign> -> "id" = <func_call>;
28) <variable_assign> -> "id"= "string";
29) <func_call> -> "id"(<params>)
30) <params> -> ε
31) <params> -> "string"<params_next>
32) <params> -> "id"<params_next>
33) <params> -> <EXPRESSION><params_next>
34) <params> -> <func_call><params_next>
35) <params_next> ->  ε
36) <params_next> ->, "string"<params_next>
37) <params_next> ->, "id"<params_next>
38) <params_next> -> ,<EXPRESSION><params_next>
39) <params_next> -> ,<func_call><params_next>
40) <if> -> if (<EXPRESSION>){<func_body>}else{<func_body>} 
41) <if> -> if (<EXPRESSION>) |"id"| {<func_body>}else{<func_body>} 
42) <while> ->while (<EXPRESSION>){<func_body>}
43) <while> ->while (<EXPRESSION>) |"id"| {<func_body>}
44) <void_call> -> "id"(<params>);
45) <return_statement> -> return <EXPRESSION>;

46) <EXPRESSION> -> <EXPRESSION> > <compared>
47) <EXPRESSION>-> <EXPRESSION> >= <compared>
48) <EXPRESSION>-> <EXPRESSION> < <compared>
49) <EXPRESSION>-> <EXPRESSION> <= <compared>
50) <EXPRESSION>-> <EXPRESSION> == <compared>
51) <EXPRESSION>-> <EXPRESSION> != <compared>
52) <EXPRESSION>-><compared>
53) <compared> -> <compared> + <term>
54) <compared> -> <compared> - <term>
55) <compared> -> <term>
56) <term> -> <term> * <factor>
57) <term> -> <term> / <factor>
58) <term> -> <factor>
59) <factor> -> "id"
60) <factor> -> "int"
61) <factor> -> "float"
62) <factor> ->  (<EXPRESSION>)