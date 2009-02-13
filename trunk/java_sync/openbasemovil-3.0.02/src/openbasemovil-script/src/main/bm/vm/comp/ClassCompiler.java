package bm.vm.comp;

/* -----------------------------------------------------------------------------
    bmScript Scripting language for Mobile Devices
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.io.SerializationException;
import bm.core.io.SerializerOutputStream;
import bm.core.math.FixedPoint;
import bm.core.tools.Tools;
import bm.core.tools.StreamParser;
import bm.core.tools.ParserException;
import bm.vm.*;
import bm.vm.lang.*;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Reader;
import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 19-oct-2007 14:41:59
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Compile a single class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ClassCompiler
{
    static final String[]   CLASS_MEMBERS   = new String[] { "property", "method", "}", ";" };
    static final int        PROPERTY        = 0;
    static final int        METHOD          = 1;
    static final String     WS              = StreamParser.WHITESPACE;
    static final String     LCASE           = "abcdefghijklmnopqrstuvwxyz";
    static final String     UCASE           = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static final String     DIGITS          = "0123456789";
    static final String     ALPHA           = LCASE + UCASE;

    private static Hashtable keywords = new Hashtable( 5 );
    static
    {
        keywords.put( "var", new VarKeyword() );
        keywords.put( "set", new SetKeyword() );
        keywords.put( "if", new IfKeyword() );
        keywords.put( "return", new ReturnKeyword() );
        keywords.put( "while", new WhileKeyword() );
    }

    private VirtualMachine          vm;
    private Reader                  in;
    private SerializerOutputStream  out;
    StreamParser parser;

    /**
     * Create a new compiler instance to compile a single class.
     *
     * @param vm virtual machine which under the compiler runs
     * @param in input reader for source
     * @param out output stream for byte codes
     */
    public ClassCompiler(
            final VirtualMachine    vm,
            final Reader            in,
            final OutputStream      out
    )
    {
        this.vm = vm;
        this.in = in;
        this.out = out instanceof SerializerOutputStream ?
                   (SerializerOutputStream) out :
                   new SerializerOutputStream( out );
    }

    /**
     * Compile the source into byte codes.
     *
     * @throws CompilerException if source has errors
     * @throws SerializationException on errors writing to output
     */
    public void compile()
            throws CompilerException,
                   SerializationException
    {
        parser = new StreamParser( in );

        try
        {
// First we want a "class" token
            String token = parser.next( WS, false, true, false );
            checkToken( token, "class" );

            // Now the class name
            token = parser.next( WS + ":{", false, false, true );
            checkValidIdentifier( token );
            final CompScriptingClass clazz = new CompScriptingClass( vm );
            clazz.setName( token.trim() );

            final char nextChar = parser.searchNextChar( ":{" );
            if( nextChar == ':' )
            {
                char delimiter = ':';
                while( delimiter != '{' )
                {
                    String superClass = parser.next( ",{", false, true, true );
                    checkValidIdentifier( superClass );
                    try
                    {
                        clazz.addSuperClass( superClass );
                    }
                    catch( ScriptingClassNotFoundException e )
                    {
                        throw new CompilerException(
                                "Class not found",
                                parser.getStartLine(),
                                parser.getStartChar()
                        );
                    }
                    delimiter = (char) parser.nextChar();
                }
            }

            // Now a "property" or "method" keyword
            token = parser.next( WS, false, true, false );
            checkToken( token, CLASS_MEMBERS );
            while( !token.equals( "}" ) )
            {
                if( token.equals( CLASS_MEMBERS[ PROPERTY ] ) )
                {
                    parseProperty( clazz );
                }
                else if( token.equals( CLASS_MEMBERS[ METHOD ] ) )
                {
                    parseMethod( clazz );
                }
                token = parser.next( WS, false, true, false );
                checkToken( token, CLASS_MEMBERS );
            }

            clazz.serialize( out );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    private void parseMethod( final CompScriptingClass clazz )
            throws CompilerException
    {
        try
        {
            String token = parser.next( WS + "(", false, false, true );
            checkValidIdentifier( token );
            ScriptingClass returnType;
            String methodName;
            if( token.equals( clazz.getName() ) )
            {
                // Constructor
                parser.skipWhiteSpace();
                int c = parser.nextChar();
                parser.pushback( c );
                if( c == '(' )
                {
                    returnType = clazz;
                    methodName = token;
                }
                else
                {
                    returnType = getClass( token );
                    token = parser.next( "(", false, false, true );
                    checkValidIdentifier( token );
                    methodName = token;
                }
            }
            else
            {
                returnType = getClass( token );
                token = parser.next( "(", false, false, true );
                checkValidIdentifier( token );
                methodName = token;
            }
            token = parser.next( WS + ALPHA + ")", false, false, true );
            checkToken( token, "(" );

            final CompMethod method = new CompMethod( clazz );
            // ToDo: do something with return type

            token = parser.next( WS + "{", false, false, true );
            if( token == null )
            {
                throw new CompilerException(
                        "Unexpected end of file",
                        parser.getStartLine(),
                        parser.getStartChar()
                );
            }
            while( token != null && !token.equals( ")" ) )
            {
                final ScriptingClass paramClass = getClass( token );
                token = parser.next( ",)", false, false, true );
                checkValidIdentifier( token );
                final String paramName = token;
                method.addArgument( paramName, paramClass );
                final int c = parser.nextChar();
                if( c == ')' )
                {
                    token = null;
                }
                else
                {
                    token = parser.next( WS, false, false, false );
                    if( token == null )
                    {
                        throw new CompilerException(
                                "Unexpected end of file",
                                parser.getStartLine(),
                                parser.getStartChar()
                        );
                    }
                }
            }
            parser.searchNext( '{' );
            final BlockKeyword keyword = new BlockKeyword();
            final Block methodBody = new Block();
            keyword.parse( this, parser, methodBody );
            method.setBody( methodBody );
            clazz.addMethod( methodName, method );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    CompExpression parseExpression( final String terminators )
            throws CompilerException
    {
        try
        {
            final CompExpression expression = new CompExpression();
            parser.skipWhiteSpace();
            int c = parser.nextChar();
            switch( c )
            {
                case -1:
                    throw new CompilerException(
                            "Unexpected end of file",
                            parser.getStartLine(),
                            parser.getStartChar()
                    );

                case '!':   // logical negation operator
                    parseNot( expression );
                    break;

                case '"':   // string start
                    final String value = parser.next( "\"", true, false, false, true );
                    final Instance instance = vm.newInstance( "String" );
                    instance.set( "value", value );
                    expression.setValue( instance );
                    break;

                case '(':   // nested binary expression
                    parseBinaryExpression( expression );
                    break;

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':   // number
                    parser.pushback( c );
                    parseNumber( expression );
                    break;

                default:    // keyword or variable reference, possibly with a method call or property access
                    parser.pushback( c );
                    parseChain( expression );
                    break;
            }
            if( terminators != null )
            {
                parser.skipWhiteSpace();
                c = parser.nextChar();
                if( terminators.indexOf( c ) == -1 )
                {
                    throw new CompilerException(
                            "Expected one of: " + terminators,
                            parser.getStartLine(),
                            parser.getStartChar()
                    );
                }
            }
            return expression;
        }
        catch( IOException e )
        {
            throw new CompilerException(
                    "Error reading file",
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        catch( ScriptingClassNotFoundException e )
        {
            throw new CompilerException(
                    "Class not found: " + e.getMessage(),
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        catch( VirtualMachineException e )
        {
            throw new CompilerException(
                    "Virtual machine exception: " + e.getMessage(),
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    private void parseChain( final CompExpression expression )
            throws CompilerException
    {
        try
        {
            String token = parser.next( WS + ";()", false, false, true );

            // It's a property access or method call
            final int dotPosition = token.indexOf( '.' );
            if( dotPosition > -1 )
            {
                String target = token.substring( 0, dotPosition );
                String targetClass = null;
                if( target.startsWith( "super:" ) )
                {
                    targetClass = target.substring( 6 );
                    target = "this";
                }
                final String member = token.substring( dotPosition + 1 );
                final CompEval eval = new CompEval();
                eval.setItem( target );
                final CompExpression expr = new CompExpression();
                expr.setCommand( eval );
                parser.skipWhiteSpace();
                final int c = parser.nextChar();
                if( c == '(' )
                {
                    // a method call
                    parseMethodCall(
                            expression,
                            expr,
                            member,
                            targetClass
                    );
                }
                else
                {
                    // a property reference
                    parser.pushback( c );
                    final CompGet get = new CompGet();
                    get.setTarget( expr );
                    get.setPropertyName( member );
                    expression.setCommand( get );
                }
            }
            else if( token.equals( "new" ) )
            {
                final CompNew compNew = new CompNew();
                token = parser.next( ";", false, false, true );
                if( token == null )
                {
                    throw new CompilerException(
                            "Class name expected",
                            parser.getStartLine(),
                            parser.getStartChar()
                    );
                }
                token = token.trim();
                if( !token.endsWith( "()") )
                {
                    throw new CompilerException(
                            "Missing parenthesis",
                            parser.getStartLine(),
                            parser.getStartChar()
                    );
                }
                compNew.setClassName( token.substring( 0, token.length() - 2 ) );
                expression.setCommand( compNew );
            }
            else if( token.trim().length() > 0 )
            {
                // It's a simple variable or property reference, or a keyword
                final CompEval eval = new CompEval();
                eval.setItem( token );
                expression.setCommand( eval );
            }
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    private void parseMethodCall(
            final CompExpression    expression,
            final CompExpression    target,
            final String            methodName,
            final String            targetClass
    )
            throws CompilerException
    {
        final CompInvocation invocation = new CompInvocation();
        invocation.setTargetClass( targetClass );
        invocation.setTarget( target );
        invocation.setMethodName( methodName );
        CompExpression arg = parseExpression( ",)" );
        while( !arg.isNull() )
        {
            invocation.addArgument( arg );
            if( parser.getLastChar() == ')' )
            {
                break;
            }
            else
            {
                arg = parseExpression( ",)" );
            }
        }
        expression.setCommand( invocation );
    }

    private void parseBinaryExpression( final CompExpression expression )
            throws CompilerException
    {
        final CompExpression operand1 = parseExpression( null );
        try
        {
            parser.skipWhiteSpace();
            int c1 = parser.nextChar();
            Command command;
            BinaryOperator operator;
            switch( c1 )
            {
                case '+':
                    operator = new Plus();
                    command = operator;
                    break;

                case '-':
                    operator = new Minus();
                    command = operator;
                    break;

                case '*':
                    operator = new Mult();
                    command = operator;
                    break;

                case '/':
                    operator = new Div();
                    command = operator;
                    break;

                case '<':
                {
                    final int c2 = parser.nextChar();
                    if( c2 != '=' )
                    {
                        parser.pushback( c2 );
                        operator = new Comparator( Comparator.LT );
                        command = operator;
                    }
                    else
                    {
                        operator = new Comparator( Comparator.LTE );
                        command = operator;
                    }
                }
                    break;

                case '>':
                {
                    final int cx = parser.nextChar();
                    if( cx != '=' )
                    {
                        parser.pushback( cx );
                        operator = new Comparator( Comparator.GT );
                        command = operator;
                    }
                    else
                    {
                        operator = new Comparator( Comparator.GTE );
                        command = operator;
                    }
                }
                    break;

                case '!':
                {
                    final int c2 = parser.nextChar();
                    if( c2 != '=' )
                    {
                        throw new CompilerException(
                                "= expected",
                                parser.getStartLine(),
                                parser.getStartChar()
                        );
                    }
                    else
                    {
                        operator = new Comparator( Comparator.DIFFERENT );
                        command = operator;
                    }
                }
                    break;

                case '&':
                case '|':
                case '=':
                    final int c3 = parser.nextChar();
                    if( c1 != c3 )
                    {
                        throw new CompilerException(
                                ((char) c1) + " expected",
                                parser.getStartLine(),
                                parser.getStartChar()
                        );
                    }
                    else
                    {
                        switch( c3 )
                        {
                            case '&':
                                operator = new And();
                                break;

                            case '|':
                                operator = new Or();
                                break;

                            default:
                                operator = new Comparator( Comparator.EQUAL );
                                break;
                        }
                        command = operator;
                    }
                    break;

                default:
                    throw new CompilerException(
                            "Operator expected",
                            parser.getStartLine(),
                            parser.getStartChar()
                    );

            }
            final CompExpression operand2 = parseExpression( ")" );
            operator.setOperand1( operand1 );
            operator.setOperand2( operand2 );
            expression.setCommand( command );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    private void parseNumber( final CompExpression expression )
            throws CompilerException
    {
        try
        {
            final String token = parser.next( WS + ";)", false, false, true );
            if( token.startsWith( "0x" ) || token.startsWith( "-0x" ) )
            {
                expression.setValue( parseHexa( token ) );
            }
            else
            {
                expression.setValue( parseDecimal( token ) );
            }
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    private Instance parseDecimal( final String token )
            throws CompilerException
    {
        Instance instance;
        Object value;
        try
        {
            if( token.indexOf( '.' ) > -1 )
            {
                value = FixedPoint.parse( token, '.' );
                instance = vm.newInstance( "double" );
            }
            else
            {
                instance = vm.newInstance( "long" );
                value = new Long( Long.parseLong( token ) );
            }
            instance.set( "value", value );
            return instance;
        }
        catch( NumberFormatException e )
        {
            throw new CompilerException(
                    "Invalid number: " + token ,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        catch( ScriptingClassNotFoundException e )
        {
            throw new CompilerException(
                    "Class not found: " + token ,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        catch( VirtualMachineException e )
        {
            throw new CompilerException(
                    "Virtual machine exception: " + e.getMessage() ,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
    }

    private Instance parseHexa( final String token )
            throws CompilerException
    {
        try
        {
            long val;
            if( token.charAt( 0 ) == '-' )
            {
                val = - Long.parseLong( token.substring( 3 ), 16 );
            }
            else
            {
                val = Long.parseLong( token.substring( 2 ), 16 );
            }
            final Instance instance = vm.newInstance( "sys.Long" );
            instance.set( "value", new Long( val ) );
            return instance;
        }
        catch( ScriptingClassNotFoundException e )
        {
            throw new CompilerException(
                    "Class not found: " + token ,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        catch( VirtualMachineException e )
        {
            throw new CompilerException(
                    "Virtual machine exception: " + e.getMessage() ,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
    }

    private void parseNot( final CompExpression expression )
            throws CompilerException, IOException
    {
        final int c;
        try
        {
            parser.skipWhiteSpace();
            c = parser.nextChar();
            if( c != '(' )
            {
                throw new CompilerException(
                        "( expected",
                        parser.getStartLine(),
                        parser.getStartChar()
                );
            }
            final CompNot not = new CompNot();
            not.setOperand( parseExpression( ")" ) );
            expression.setCommand( not );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    Keyword getKeyword( final String name )
            throws CompilerException
    {
        return (Keyword) keywords.get( name );
    }

    private void parseProperty( final CompScriptingClass clazz )
            throws CompilerException
    {
        try
        {
            String token = parser.next( WS, false, false, false );
            final ScriptingClass propClass = getClass( token );

            token = parser.next( ";", false, false, false );
            checkValidIdentifier( token );

            clazz.addProperty( token, propClass );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }

    ScriptingClass getClass( final String token )
            throws CompilerException
    {
        checkValidIdentifier( token );
        try
        {
            return vm.getClassLoader().load( token );
        }
        catch( ScriptingClassNotFoundException e )
        {
            throw new CompilerException(
                    "Target class not found: " + token,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
    }

    private void checkValidIdentifier( final String token )
            throws CompilerException
    {
        if( token == null )
        {
            throw new CompilerException(
                    "Unexpected end of file",
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        if( !StreamParser.isValidIdentifier( token.trim() ) )
        {
            throw new CompilerException(
                    "Invalid identifier: " + token,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
    }

    private void checkToken( final String token, final String[] values )
            throws CompilerException
    {
        if( token == null )
        {
            throw new CompilerException(
                    "Unexpected end of file",
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        final int count = values != null ? values.length : 0;
        for( int i = 0; i < count; i++ )
        {
            //noinspection ConstantConditions
            if( token.equals( values[i] ) )
            {
                return;
            }
        }
        throw new CompilerException(
                "Expected one of: " + Tools.arrayAsVector( values ),
                parser.getStartLine(),
                parser.getStartChar()
        );

    }

    private void checkToken( final String token, final String value )
            throws CompilerException
    {
        if( token == null )
        {
            throw new CompilerException(
                    "Unexpected end of file",
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
        if( !token.equals( value ) )
        {
            throw new CompilerException(
                    "Expected: " + value,
                    parser.getStartLine(),
                    parser.getStartChar()
            );
        }
    }
}
