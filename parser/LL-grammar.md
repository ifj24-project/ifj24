1) <start> -> <program_prolog><program>
2) <program_prolog> -> const "id" = @import("ifj24.zig");
3) <program> -> <func_define><program>
4) <program> -> ε

5) <id> -> id
6) <int>-> int
7) <float> -> float

8) <data_type> -> i32
9) <data_type> -> f64
10) <data_type> -> []u8
11) <data_type> -> void

12) <func_define> -> pub fn <id>(<params_define>) <data_type>{<func_body>}
13) <params_define> -> ε
14) <params_define> -> <id>: <data_type><params_define_next>
15) <params_define_next> -> ε
16) <params_define_next> -> ,<id>: <data_type><params_define_next>
17) <func_body> ->ε
18) <func_body> -> <statement><func_body>

19) <statement> -> <variable_define>
20) <statement> -> <variable_assign>
21) <statement> -> <if>
22) <statement> -> <while>
23) <statement> -> <void_call>
24) <statement> -> <return_statement>

25) "rhs" -> <id>
26) "rhs" -> <func_call>
27) "rhs" -> <EXPRESSION>
28) "rhs" -> <string>

29) <variable_define> -> const <id> : <data_type> = "rhs";
30) <variable_define> -> var <id>: <data_type> = "rhs";
31) <variable_assign> -> <id> = "rhs";
32) <func_call> -> <id>(<params>)
33) <params> -> ε
34) <params> -> "rhs"<params_next>
35) <params_next> ->  ε
36) <params_next> ->, "rhs"<params_next>
37) <if> -> if (<EXPRESSION>){<func_body>}else{<func_body>}
38) <if> -> if (<EXPRESSION>) |<id>| {<func_body>}else{<func_body>}
39) <while> ->while (<EXPRESSION>){<func_body>}
40) <while> ->while (<EXPRESSION>) |<id>| {<func_body>}
41) <void_call> -> <id>(<params>);
42) <return_statement> -> return;
43) <return_statement> -> return "rhs";

44) <EXPRESSION> -> "zpracovano pomoci precedencni analyzy"
