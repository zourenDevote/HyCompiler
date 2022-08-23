/*
 * Author:  Devotes
 * Date:    2022/5/15
 *
 * FixLog:
 *
 *
*/

lexer grammar Token;

options{ language = Cpp;}

/// Token Define

// variable
Int:            'int';
Float:          'float';
Void:           'void';

// key word
Const:          'const';
If:             'if';
Else:           'else';
While:          'while';
Break:          'break';
Continue:       'continue';
Return:         'return';

Assign:         '=';
Lsb:            '[';
Rsb:            ']';
SLp:            '(';
SRp:            ')';

// arithmetic operator
Add:            '+';
Sub:            '-';
Mul:            '*';
Div:            '/';
Mod:            '%';

// relational operator
Equal:          '==';           // equal
Nequal:         '!=';           // not equal
Gt:             '>';            // gather than
Lt:             '<';            // less than
Le:             '<=';           // less than or equal than
Ge:             '>=';           // gather than or equal than

// logic operator
Not:            '!';            // not
And:            '&&';           // and
Or:             '||';           // or


// ident
Id:             Id_nondigit Identifier*;

fragment Id_nondigit:    [a-zA-Z_];

fragment Identifier:     [a-z0-9A-Z_];

// Int Const
IntConst:         Decimal_const
                | Octal_const
                | Hexadecimal_const;

fragment Decimal_const:  [1-9] [0-9]* | '0' ;

fragment Octal_const:    '0' ([0-7] [0-8]* | '0' ) ;

fragment Hexadecimal_const: Hex_prefix [0-9a-fA-F]+;

fragment Hex_prefix:    ('0x'|'0X');

// Float Const

FloatConst:       Decimal_floating_const
                | Hexadecimal_floating_const;

fragment Decimal_floating_const:             Fractional_const Exponent_part?
                                           | Digit_sequence Exponent_part;

fragment Fractional_const:                   Digit_sequence? '.' Digit_sequence
                                           | Digit_sequence '.';

fragment Exponent_part:                      ( 'E' | 'e' ) ( '+' | '-' )? Digit_sequence;

fragment Digit_sequence:                     [0-9]+;

fragment Hexadecimal_floating_const:         Hex_prefix Hexacdecmial_fractional_constant Binary_exponent_part?
                                           | Hex_prefix Hexadecimal_digit_sequence Binary_exponent_part?;

fragment Hexacdecmial_fractional_constant:   Hexadecimal_digit_sequence? '.' Hexadecimal_digit_sequence
                                           | Hexadecimal_digit_sequence '.';

fragment Binary_exponent_part:               ('p' | 'P') ('+' | '-')? Digit_sequence;

fragment Hexadecimal_digit_sequence:         [a-fA-F0-9]+;


// white space
Ws:             [ \r\n\t]+ -> skip;


BlockComment
    :   '/*' .*? '*/' -> skip
    ;

LineComment
    :   '//' ~[\r\n]*
        -> skip
    ;

Eof:                EOF;