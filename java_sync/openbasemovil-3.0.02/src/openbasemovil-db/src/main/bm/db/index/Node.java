/*
 * Copyright (c) 2005 Elondra, S.L. All Rights Reserved.
 */
package bm.db.index;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Database Library
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

import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.db.DBException;

import bm.storage.RecordStoreFullException;

/*
 * File Information
 *
 * Created on       : 06-jul-2005 13:06:42
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2007-05-15 03:11:14 +0200 (mar, 15 may 2007) $
 * Revision         : $Revision: 6 $
 */

/**
 * A BTree index node.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class Node
    implements Serializable
{
    private int         keyCount;
    private NodeKey[]   keys;
    private int[]       children;
    private boolean     leaf      = true;

    private transient Index       index;
    private transient int         order;
    private transient Integer     recordId;
    private transient Node        parent;


    public Node( final Index index, final int order, final Node parent )
    {
        this.index  = index;
        this.order  = order;
        this.parent = parent;
        // BTree nodes can have up to 2*order keys and 2*order + 1 children
        // We allocate an extra slot on both arrays to simplify node splitting
        keys        = new NodeKey[2 * order + 1];
        children    = new int[2 * order + 2];
        leaf      = true;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.db.index.Node";
    }

    /**
     * The record id of this node on persistent storage. When null the node is
     * new.
     *
     * @return record id
     */
    public Integer getRecordId()
    {
        return recordId;
    }

    /**
     * Set the record id.
     *
     * @param recordId record id
     */
    public void setRecordId( final Integer recordId )
    {
        this.recordId = recordId;
    }

    /**
     * Write node to output stream.
     *
     * @param out output stream
     * @throws SerializationException on errors
     */
    public void serialize( final SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // Version
        out.writeInt( keyCount );
        for( int i = 0; i < keyCount; i++ )
        {
            keys[i].serialize( out );
        }
        final int count = children.length - 1;
        for( int i = 0; i < count; i++ )
        {
            out.writeInt( children[i] );
        }
        out.writeBoolean( leaf );
    }

    /**
     * Read node from input stream.
     *
     * @param in input stream
     * @throws SerializationException on errors
     */
    public void deserialize( final SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // Skip version
        keyCount = in.readInt();
//            log.debug( "keyCount: " + keyCount );
        for( int i = 0; i < keyCount; i++ )
        {
            keys[i] = new NodeKey( index );
            keys[i].deserialize( in );
        }
        final int count = children.length - 1;
        for( int i = 0; i < count; i++ )
        {
            children[i] = in.readInt();
        }
        leaf = in.readBoolean();
    }

    void setKey( final int i, final NodeKey key )
    {
        keys[ i ] = key;
        index.addToSave( this );
    }

    void setChild( final int i, final int child )
    {
        children[ i ] = child;
        index.addToSave( this );
    }

    int getChild( final int i )
    {
        return children[ i ];
    }

    void setParent( final Node parent )
    {
        this.parent = parent;
    }

    Node getParent()
    {
        return parent;
    }

    void setLeaf( final boolean value )
    {
        leaf = value;
        index.addToSave( this );
    }

    boolean isLeaf()
    {
        return leaf;
    }

    NodeKey getKey( final int i )
    {
        return keys[ i ];
    }

    int getKeyCount()
    {
        return keyCount;
    }

    void incKeyCount()
    {
        keyCount++;
        index.addToSave( this );
    }

    void setKeyCount( final int keyCount )
    {
        this.keyCount = keyCount;
        index.addToSave( this );
    }

    Index getIndex()
    {
        return index;
    }

    /**
     * Split the node in two.<br/>This can be done only when the node is full.
     * The middle key will go up to the parent, the left keys will remain in
     * this node, and the right keys will go to a new &quot;brother&quote; node
     * at the right of this one. If the node is the root node the split is
     * always done into two new nodes, left and right of the root one which will
     * keep just the middle key.
     * @noinspection FieldRepeatedlyAccessedInMethod
     * @throws SerializationException on errors
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space is left on record store
     */
    void split()
            throws DBException,
                   SerializationException,
                   RecordStoreFullException
    {
        if( keyCount == keys.length )
        {
            if( parent == null )
            {
                // This is the root node
                final Node left = new Node( index, order, this );
                final Node right = new Node( index, order, this );
                // Divide the keys between the nodes
                for( int i = 0; i < order; i++ )
                {
                    left.setKey( i, keys[i] );
                    right.setKey( i, keys[order + i + 1] );
                }
                // if it's not a leaf divide also children
                if( !leaf )
                {
                    for(int i = 0; i <= order; i++)
                    {
                        left.setChild( i, children[i] );
                        final Node leftChild = index.getNode( this, children[i] );
                        leftChild.setParent( left );
                        final Node rightChild = index.getNode(
                                this,
                                children[order + i + 1]
                        );
                        right.setChild( i, children[order + i + 1] );
                        rightChild.setParent( right );
                    }
                    left.setLeaf( false );
                    right.setLeaf( false );
                }
                else
                {
                    setLeaf( false );
                }
                // shrink the node
                keys[0] = keys[order];
                keyCount = 1;
                left.keyCount = order;
                right.keyCount = order;
                final int keyLength = keys.length;
                for(int i = 1; i < keyLength; i++)
                {
                    keys[i] = null;
                    children[i + 1] = 0;
                }
                index.saveNode( left );
                index.saveNode( right );
                // Add the new children to the node
                children[0] = left.getRecordId().intValue();
                children[1] = right.getRecordId().intValue();
                index.addToSave( this );
            }
            else
            {
                // This is a child node
                final Node right = new Node( index, order, parent );
                System.arraycopy( keys, order + 1, right.keys, 0, order );
                right.keyCount = order;
                if( !leaf )
                {
                    for( int i = 0; i <= order; i++ )
                    {
                        right.children[i] = children[order + i + 1];
                        final Node rightChild = index.getNode(
                                this,
                                right.children[i]
                        );
                        rightChild.parent = right;
                    }
                    right.leaf = false;
                }
                index.saveNode( right );

                final NodeKey bubble = keys[order];
                int i;
                final int keyLength = keys.length;
                for( i = order; i < keyLength; i++ )
                {
                    keys[i] = null;
                    children[i+1] = 0;
                }
                keyCount = order;
                parent.insert( bubble, right.getRecordId() );
                index.addToSave( this );
            }
        }
    }

    /**
     * Shift the arrays of keys and children one position to the right, making
     * place for a new key. Key count is NOT incremented.
     *
     * @param index start position
     */
    void shift( final int index )
    {
        // Local variables to improve speed on CDC
        final NodeKey[] keys = this.keys;
        final int[] children = this.children;
        System.arraycopy(
                keys,
                index,
                keys,
                index + 1,
                keyCount - index
        );
        if( !leaf )
        {
            System.arraycopy(
                    children,
                    index,
                    children,
                    index + 1,
                    keyCount - index + 1
            );
        }
        this.index.addToSave( this );
    }

    /**
     * Insert a new key in the right position (ascending order).
     *
     * @param nodeKey new node key
     * @param rightChild right child id
     * @noinspection FieldRepeatedlyAccessedInMethod
     * @throws SerializationException on errors
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space is left on record store
     */
    void insert( final NodeKey nodeKey, final Integer rightChild )
            throws DBException,
                   SerializationException,
                   RecordStoreFullException
    {
        if( keyCount == 0 )
        {
            keyCount++;
            keys[0] = nodeKey;
        }
        else
        {
            int index = 0;
            //noinspection MethodCallInLoopCondition
            while( nodeKey.compareTo( keys[index] ) > 0 )
            {
                index++;
                if( index == keyCount )
                {
                    break;
                }
            }
            if( index <= (keyCount - 1) )
            {
                shift( index );
            }
            keys[index] = nodeKey;
            keyCount++;
            if( rightChild != null )
            {
                children[index+1] = rightChild.intValue();
            }
            if( keyCount == keys.length )
            {
                split();
            }
        }
        index.addToSave( this );
    }

    /**
     *
     * @param neighbour neighbour node
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    void appendNode( final Node neighbour )
    {
        System.arraycopy(
                neighbour.keys,
                0,
                keys,
                order,
                neighbour.getKeyCount()
        );
        if( !neighbour.isLeaf() )
        {
            System.arraycopy(
                    neighbour.children,
                    0,
                    children,
                    order,
                    neighbour.getKeyCount() + 1
            );
        }
        keyCount = 2 * order;
        index.addToSave( this );
    }

    /**
     *
     * @param neighbour neighbour node
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    void insertNode( final Node neighbour )
    {
        System.arraycopy( keys, 0, keys, order, order );
        System.arraycopy( children, 0, children, order, order + 1 );
        System.arraycopy( neighbour.keys, 0, keys, 0, order );
        if( !neighbour.leaf )
        {
            System.arraycopy( neighbour.children, 0, children, 0, order + 1 );
        }
        keyCount = 2 * order;
        index.addToSave( this );
    }

    void moveToParent()
    {
        final Node parent = this.parent;
        final int keyCount = this.keyCount;
        System.arraycopy( keys, 0, parent.keys, 0, keyCount );
        if( !leaf )
        {
            System.arraycopy( children, 0, parent.children, 0, keyCount + 1 );
        }
        else
        {
            // Clear child array
            final int length = parent.children.length;
            for( int i = 0; i < length; i++ )
            {
                parent.children[i] = 0;
            }
        }
        parent.setLeaf( leaf );
        parent.setKeyCount( keyCount );
    }

    /**
     * Remove a key from this node.
     *
     * @param keyIndex key index
     * @return the removed key
     * @noinspection FieldRepeatedlyAccessedInMethod
     * @throws DBException on errors
     */
    NodeKey extractNodeKey( final int keyIndex )
            throws DBException
    {
        final NodeKey tmpNodeKey = getNodeKey( keyIndex );
        System.arraycopy(
                keys,
                keyIndex + 1,
                keys,
                keyIndex,
                keyCount - keyIndex - 1
        );
        keys[keyCount - 1] = null; // Inocua
        if( !leaf )
        {
            System.arraycopy(
                    children,
                    keyIndex + 1,
                    children,
                    keyIndex,
                    keyCount - keyIndex
            );
            children[ keyCount ] = 0; // Inocua
        }
        keyCount--;
        if( keyCount == 0 )
        {
            // Clear last child entry when no keys left
            children[0] = 0;
        }
        index.addToSave( this );
        return tmpNodeKey;
    }

    /**
     * Get the key at a given index.
     *
     * @param keyIndex key index
     * @return key
     */
    NodeKey getNodeKey( final int keyIndex )
    {
        return keys[keyIndex];
    }

    /**
     * Get the child node at the given index.
     * @param keyIndex key index
     * @return child node
     * @throws DBException on errors
     */
    Node getChildNode( final int keyIndex )
            throws DBException
    {
        return index.getNode( this, children[ keyIndex ] );
    }

    int size()
            throws DBException
    {
        int size = keyCount;
        if( !leaf )
        {
            for( int i = 0; i <= keyCount; i++ )
            {
                size += getChildNode( i ).size();
            }
        }
        return size;
    }

    public String toString()
    {
        // Local variable to improve speed on CDC
        final int keyCount = this.keyCount;
        final StringBuffer buffer = new StringBuffer( "Node{" )
                .append( "keyCount=" ).append( keyCount )
                .append( ",leaf=" ).append( leaf )
                .append( ",recordId=" ).append( recordId );
        buffer.append( ",keys={" );
        for( int i = 0; i < keyCount; i++ )
        {
            buffer.append( "key[" ).append( i ).append( "]=" )
                    .append( keys[i].getKey() )
                    .append( ", " )
                    .append( keys[i].getData() );
            if( i < keyCount - 1 )
            {
                buffer.append( "," );
            }
        }
        buffer.append( "}" );
        if( !leaf )
        {
            buffer.append( ",children={" );
            for( int i = 0; i <= keyCount; i++ )
            {
                buffer.append( "child[" ).append( i ).append( "]=" )
                        .append( children[i] );
                if( i < keyCount )
                {
                    buffer.append( "," );
                }
            }
            buffer.append( "}" );
        }
        buffer.append( "}" );
        return buffer.toString();
    }
}
