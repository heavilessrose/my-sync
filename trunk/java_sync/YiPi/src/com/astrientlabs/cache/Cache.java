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
package com.astrientlabs.cache;

public class Cache
{
    public static Cache instance;
    
    private Object[] model;
    private Object[] keys;
    
    public Cache(int size)
    {
        model = new Object[size];
        keys = new Object[size];
    }
    
    
    public int slots()
    {
        return model.length;
    }
    
    public void put(Object key, Object value)
    {
        int pos = Math.abs(key.hashCode() % model.length); //% model.length);
        
        keys[pos] = key;
        model[pos] = value;   
    }
    
    public Object get(Object key)
    {
        int pos =  Math.abs(key.hashCode() % model.length ); //% model.length);
        
        //TODO: key == key?
        if ( key.equals(keys[pos]) )
        {
            return model[pos];
        }
        
        return null;
    }
    
    public int size()
    {
        int count = 0;
        for ( int i = 0; i < model.length; i++ )
        {
            if ( model[i] != null ) count++;
        }
        
        return count;
    }
    
    public void clear()
    {
        for (int i = 0; i < model.length; i++)
        {
            model[i] = null;
            keys[i] = null;
        }
    }
    
    
}
