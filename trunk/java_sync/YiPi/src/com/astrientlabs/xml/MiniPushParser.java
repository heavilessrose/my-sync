/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.xml;

import java.io.IOException;
import java.io.InputStream;

public class MiniPushParser
{
    private StringBuffer tag = new StringBuffer();
    private StringBuffer attribute = new StringBuffer();
    private StringBuffer value = new StringBuffer();
    
    private static final int MODE_TAG = 0;
    private static final int MODE_ATTRIBUTE = 1;
    private static final int MODE_ATTRIBUTE_VALUE = 2;
    private static final int MODE_VALUE = 3;
    private static final int MODE_ENDTAG = 4;
    
    private int mode = -1;
    private char preceeding = '\0';
    private boolean startHandled = false;
    private boolean cancel;
    
    public MiniPushParser()
    {
    }
    
    public void cancel()
    {
        cancel = true;
    }
    
    public void parse(InputStream is, MiniPushHandler handler, int bufferSize) throws IOException
    {       
        cancel = false;
        
        byte[] buffer = new byte[bufferSize];
        
        char b;
        int i;
        
        boolean skip = false;
      
        
        while ( !cancel && ((i = is.read(buffer)) != -1) )
        {
            for ( int j = 0; j < i; j++ )
            {
            
            b = (char) buffer[j];
            if ( skip )
            {
                skip = false;
                continue;
            }
            
            //System.out.print(b);
            
            if ( mode == MODE_VALUE && b == '<' )
            {
                mode = MODE_TAG;
                startHandled = false;
                if ( value.length() > 0 ) handler.handleText(tag.toString(),value.toString());
                value.setLength(0);
                tag.setLength(0);
            }
            else if ( b == '<' )
            {
                startHandled = false;
                mode = MODE_TAG;
                tag.setLength(0);
                preceeding = '<';
            }
            else if ( mode == MODE_TAG && b == ' ' )
            {
                mode = MODE_ATTRIBUTE;
                if ( !startHandled )
                {
                    handler.handleStartTag(tag.toString());
                    startHandled = true;
                }
            }
            else if ( mode == MODE_TAG && b == '/' )
            {
                if ( preceeding == '<' )
                {
                 preceeding = '\0';   
                 mode = MODE_ENDTAG;
                }
                else if ( tag.length() > 0 )
                {
                skip = true;//is.read(); //consume >
                handler.handleEndTag(tag.toString());
                tag.setLength(0);
                }
            }
            else if ( mode == MODE_ATTRIBUTE && b == '/' )
            {
                if ( tag.length() > 0 )
                {
                    skip = true;//is.read(); //consume >
                    handler.handleEndTag(tag.toString());
                    tag.setLength(0);
                }
            }
            else if ( mode == MODE_ATTRIBUTE && b == '=' )
            {
                mode = MODE_ATTRIBUTE_VALUE;
                skip = true;//is.read(); //consume qoute
            }
            else if ( mode == MODE_ATTRIBUTE_VALUE && b == '"' )
            {
                mode = MODE_TAG;
                handler.handleAttribute(tag.toString(),attribute.toString(),value.toString());
                value.setLength(0);
                attribute.setLength(0);
            }
            else if ( mode == MODE_ENDTAG && b == '>' )
            {
                handler.handleEndTag(tag.toString());
                tag.setLength(0);
                startHandled = false;
            }
            else if ( b == '>' )
            {
                mode = MODE_VALUE;
                if ( !startHandled )
                {
                    handler.handleStartTag(tag.toString());
                    startHandled = true;
                }
            }
            else
            {
                switch (mode)
                {
                    case MODE_ENDTAG : tag.append(b); break;
                    case MODE_TAG : tag.append(b); break;
                    case MODE_ATTRIBUTE : attribute.append(b); break;
                    case MODE_ATTRIBUTE_VALUE : value.append(b); break;
                    case MODE_VALUE : value.append(b); break;
                    default: break;
                }
            }
            }
        }
    }
}