#include "SlimXml.h"
#include <cassert>
#include <fstream>

namespace slim
{

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode::XmlNode( NodeType type, XmlNode* parent )
	:	m_type( type )
	,	m_parent( parent )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode::~XmlNode()
{
	clearAttribute();
	clearChild();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode* XmlNode::findChild( const Char* name ) const
{
	assert( name != NULL );
	for ( std::list<XmlNode*>::const_iterator iter = m_children.begin();
		  iter != m_children.end();
		  ++iter )
	{
		XmlNode* child = *iter;
		assert( child != NULL );
		if ( child->m_name == name )
		{
			return child;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode* XmlNode::findFirstChild( const Char* name, NodeIterator& iter ) const
{
	assert( name != NULL );
	iter = m_children.begin();
	while ( iter != m_children.end() )
	{
		XmlNode* child = *iter;
		assert( child != NULL );
		if ( child->m_name == name )
		{
			return child;
		}
		++iter;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode* XmlNode::findNextChild( const Char* name, NodeIterator& iter ) const
{
	assert( name != NULL );
	if ( iter != m_children.end() )
	{
		while ( ++iter != m_children.end() )
		{
			XmlNode* child = *iter;
			assert( child != NULL );
			if ( child->m_name == name )
			{
				return child;
			}
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
size_t XmlNode::getChildCount( const Char* name ) const
{
	assert( name != NULL );
	
	size_t count = 0;
	for ( NodeIterator iter = m_children.begin();
		iter != m_children.end();
		++iter )
	{
		XmlNode* child = *iter;
		assert( child != NULL );
		if ( child->m_name == name )
		{
			++count;
		}
	}
	return count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlNode::removeChild( XmlNode* node )
{
	assert( node != NULL );
	for ( std::list<XmlNode*>::iterator iter = m_children.begin();
		iter != m_children.end();
		++iter )
	{
		if ( *iter == node )
		{
			delete node;
			m_children.erase( iter );
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlNode::clearChild()
{
	for ( std::list<XmlNode*>::iterator iter = m_children.begin();
		  iter != m_children.end();
		  ++iter )
	{
		XmlNode* child = *iter;
		assert( child != NULL );
		delete child;
	}
	m_children.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlNode* XmlNode::addChild( const Char* name, NodeType type )
{
	if ( type != COMMENT && type != ELEMENT )
	{
		return NULL;
	}
	XmlNode* child = new XmlNode( type, this );
	if ( name != NULL )
	{
		child->m_name = name;
	}
	m_children.push_back( child );

	return child;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::addAttribute( const Char* name, const Char* value )
{
	XmlAttribute* attribute = new XmlAttribute;
	if ( name != NULL )
	{
		attribute->setName( name );
	}
	if ( value != NULL )
	{
		attribute->setString( value );
	}
	m_attributes.push_back( attribute );
	return attribute;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::addAttribute( const Char* name, const String& value )
{
	return addAttribute( value.c_str() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::addAttribute( const Char* name, bool value )
{
	XmlAttribute* attribute = addAttribute( name );
	attribute->setBool( value );
	return attribute;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::addAttribute( const Char* name, int value )
{
	XmlAttribute* attribute = addAttribute( name );
	attribute->setInt( value );
	return attribute;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::addAttribute( const Char* name, float value )
{
	XmlAttribute* attribute = addAttribute( name );
	attribute->setFloat( value );
	return attribute;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::addAttribute( const Char* name, double value )
{
	XmlAttribute* attribute = addAttribute( name );
	attribute->setDouble( value );
	return attribute;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlAttribute* XmlNode::findAttribute( const Char* name ) const
{
	for ( std::list<XmlAttribute*>::const_iterator iter = m_attributes.begin();
		  iter != m_attributes.end();
		  ++iter )
	{
		XmlAttribute* attribute = *iter;
		assert( attribute != NULL );
		if ( Strcmp( attribute->getName(), name ) == 0 )
		{
			return attribute;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const Char* XmlNode::readAttributeAsString( const Char* name, const Char* defaultValue ) const
{
	XmlAttribute* attribute = findAttribute( name );
	if ( attribute == NULL )
	{
		return defaultValue;
	}
	return attribute->getString();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlNode::readAttributeAsBool( const Char* name, bool defaultValue ) const
{
	XmlAttribute* attribute = findAttribute( name );
	if ( attribute == NULL )
	{
		return defaultValue;
	}
	return attribute->getBool();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int XmlNode::readAttributeAsInt( const Char* name, int defaultValue ) const
{
	XmlAttribute* attribute = findAttribute( name );
	if ( attribute == NULL )
	{
		return defaultValue;
	}
	return attribute->getInt();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long XmlNode::readAttributeAsHex( const Char* name, unsigned long defaultValue ) const
{
	XmlAttribute* attribute = findAttribute( name );
	if ( attribute == NULL )
	{
		return defaultValue;
	}
	return attribute->getHex();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
float XmlNode::readAttributeAsFloat( const Char* name, float defaultValue ) const
{
	XmlAttribute* attribute = findAttribute( name );
	if ( attribute == NULL )
	{
		return defaultValue;
	}
	return attribute->getFloat();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double XmlNode::readAttributeAsDouble( const Char* name, double defaultValue ) const
{
	XmlAttribute* attribute = findAttribute( name );
	if ( attribute == NULL )
	{
		return defaultValue;
	}
	return attribute->getDouble();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlNode::removeAttribute( XmlAttribute* attribute )
{
	assert( attribute != NULL );
	for ( std::list<XmlAttribute*>::iterator iter = m_attributes.begin();
		iter != m_attributes.end();
		++iter )
	{
		if ( *iter == attribute )
		{
			delete attribute;
			m_attributes.erase( iter );
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlNode::clearAttribute()
{
	for ( std::list<XmlAttribute*>::iterator iter = m_attributes.begin();
		iter != m_attributes.end();
		++iter )
	{
		delete *iter;
	}
	m_attributes.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlNode::writeNode( String& output, int depth ) const
{
	if ( depth < 0 )
	{
		//root node is not a real node, it has nothing to write
		return writeChildNodes( output, depth );
	}
	//add tabs
	int iTabCount = depth;
	while ( iTabCount-- > 0 )
	{
		output += T('	');
	}
	//comment node is special. it has no children, and has no attributes either
	if ( m_type == COMMENT )
	{
		output += T("<!--");
		output += m_name;
		output += T("-->\r\n");
		return;
	}
	//start label
	output += T('<');
	output += m_name;
	//write attributes
	for ( std::list<XmlAttribute*>::const_iterator iter = m_attributes.begin();
		  iter != m_attributes.end();
		  ++iter )
	{
		XmlAttribute* attribute = *iter;
		assert( attribute != NULL );
		output += T(' ');
		output += attribute->getName();
		output += T("=\"");
		output += attribute->getString();
		output += T('"');
	}

	if ( isEmpty() )
	{
		output += T("/>\r\n");
		return;
	}
	output += T(">");

	if ( hasChild() )
	{
		output += T("\r\n");
		writeChildNodes( output, depth );
		iTabCount = depth;
		while ( iTabCount-- > 0 )
		{
			output += T('	');
		}
	}
	else
	{
		//leaf node
		output += m_value;
	}
	//end label
	output += T("</");
	output += m_name.c_str();
	output += T(">\r\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlNode::writeChildNodes( String& output, int depth ) const
{
	for ( std::list<XmlNode*>::const_iterator iter = m_children.begin();
		  iter != m_children.end();
		  ++iter )
	{
		XmlNode* child = *iter;
		assert( child != NULL );
		child->writeNode( output, depth + 1 );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//class CXmlDocument
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
XmlDocument::XmlDocument()
	:	XmlNode( DOCUMENT, NULL )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlDocument::loadFromFile( const Char* filename )
{
	assert( filename != NULL );
	std::fstream file;
	file.open( filename, std::ios_base::in | std::ios_base::binary );
	if ( !file.is_open() )
	{
		return false;
	}
	bool succeeded = loadFromStream( file );
	file.close();
	return succeeded;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlDocument::loadFromStream( std::istream& input )
{
	input.seekg( 0, std::ios::end );
	size_t size = static_cast<size_t>( input.tellg() );
	input.seekg( 0, std::ios::beg );

	char* buffer = new char[size];
	input.read( buffer, size );

	bool succeeded = loadFromMemory( buffer, size );
	delete[] buffer;
	return succeeded;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlDocument::loadFromMemory( const char* buffer, size_t size )
{
	clearChild();

	if ( size < 3 )
	{
		return false;
	}
	//get encode
	Encode encode = ANSI;
	bool multiBytes = false;
	const unsigned char* bom = reinterpret_cast<const unsigned char*>( buffer );
	if ( bom[0] == 0xfe && bom[1] == 0xff )
	{
		encode = UTF_16_BIG_ENDIAN;
	}
	else if ( bom[0] == 0xff && bom[1] == 0xfe )
	{
		encode = UTF_16;
	}
	else if ( bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf )
	{
		encode = UTF_8;
	}
	else
	{
		encode = detectEncode( buffer, size, multiBytes );
	}

	const Char* text;	//for parser
	Char* tempBuffer = NULL;	
	size_t characterCount;

#ifdef SLIM_USE_WCHAR
	if ( encode == UTF_16 )
	{
		//skip bom
		characterCount = ( size - 2 ) / 2;
		text = reinterpret_cast<const wchar_t*>( buffer + 2 );
	}
	else if ( encode == UTF_16_BIG_ENDIAN )
	{
		//swap. can be faster
		characterCount = ( size - 2 ) / 2;
		tempBuffer = new wchar_t[characterCount];
		const wchar_t* src = reinterpret_cast<const wchar_t*>( buffer + 2 );
		const wchar_t* srcEnd = src + characterCount;
		wchar_t* dst = const_cast<wchar_t*>( tempBuffer );
		for ( ; src < srcEnd; ++src, ++dst )
		{
			*( (char*)dst ) = *( ((const char*)src) + 1 );
			*( ((char*)dst) + 1 ) = *( (const char*)src );
		}
		text = tempBuffer;
	}
	else if ( encode == UTF_8 || encode == UTF_8_NO_MARK || ( encode == ANSI && !multiBytes ) )
	{
		if ( encode == UTF_8 )
		{
			//skip bom
			buffer += 3;
			size -= 3;
		}
		tempBuffer = new wchar_t[size];
		characterCount = utf8toutf16( buffer, size, tempBuffer, size );
		text = tempBuffer;
	}
	else
	{
		return false;
	}
#elif defined(UNICODE)
	if ( encode == UTF_8 )
	{
		//skip bom
		characterCount = size - 3;
		text = buffer + 3;
	}
	else if ( encode == UTF_16 )
	{
		characterCount = ( size - 2 ) / 2;
		tempBuffer = new char[characterCount * 4];
		//it's not really character count here
		characterCount = utf16toutf8( reinterpret_cast<const wchar_t*>( buffer + 2 ), characterCount, tempBuffer, characterCount * 4 );
		text = tempBuffer;
	}
	else if ( encode == UTF_16_BIG_ENDIAN )
	{
		characterCount = ( size - 2 ) / 2;
		//swap
		wchar_t* leBuffer = new wchar_t[characterCount];
		const wchar_t* src = reinterpret_cast<const wchar_t*>( buffer + 2 );
		const wchar_t* srcEnd = src + characterCount;
		wchar_t* dst = leBuffer;
		for ( ; src < srcEnd; ++src, ++dst )
		{
			*( (char*)dst ) = *( ((const char*)src) + 1 );
			*( ((char*)dst) + 1 ) = *( (const char*)src );
		}
		tempBuffer = new char[characterCount * 4];
		//it's not really character count here
		characterCount = utf16toutf8( leBuffer, characterCount, tempBuffer, characterCount * 4 );
		text = tempBuffer;
		delete[] leBuffer;
	}
	else if ( encode == UTF_8_NO_MARK )
	{
		characterCount = size;
		text = buffer;
	}
	else
	{
		return false;
	}
#else
	if ( encode != ANSI )
	{
		return false;
	}
	characterCount = size;
	text = static_cast<const char*>( buffer );
#endif	//#ifdef SLIM_USE_WCHAR

	bool succeeded = parse( text, characterCount );

	if ( tempBuffer != NULL )
	{
		delete[] tempBuffer;
	}
	return succeeded;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlDocument::save( const Char* filename, Encode encode ) const
{
	assert( filename != NULL );

	String output;

#ifdef SLIM_USE_WCHAR
	if ( encode == ANSI )
	{
		return false;
	}
	if ( encode == UTF_16 || encode == UTF_16_BIG_ENDIAN )
	{
		output = T("<?xml version=\"1.0\" encode=\"UTF-16\"?>\r\n");
	}
	else if ( encode == UTF_8 || encode == UTF_8_NO_MARK )
	{
		output = T("<?xml version=\"1.0\" encode=\"UTF-8\"?>\r\n");
	}
#elif defined (UNICODE)
	if ( encode == UTF_8 || encode == UTF_8_NO_MARK )
	{
		output = T("<?xml version=\"1.0\" encode=\"UTF-8\"?>\r\n");
	}
	else
	{
		return false;
	}
#else
	if ( encode != ANSI )
	{
		return false;
	}
	//don't know how to get encode string of current code page
	output = T("<?xml version=\"1.0\"?>\r\n");
#endif

	writeNode( output, -1 );

	std::fstream file;
	file.open( filename, std::ios_base::out | std::ios_base::binary );
	if ( !file.is_open() )
	{
		return false;
	}

#ifdef SLIM_USE_WCHAR
	unsigned char bom[3];
	if ( encode == UTF_16 )
	{
		bom[0] = 0xff;
		bom[1] = 0xfe;
		file.write( (char*)bom, 2 );
		file.write( (char*)output.c_str(), output.size() * 2 );
	}
	else if ( encode == UTF_16_BIG_ENDIAN )
	{
		bom[0] = 0xfe;
		bom[1] = 0xff;
		file.write( (char*)bom, 2 );
		//swap
		wchar_t* buffer = new wchar_t[output.size()];
		const wchar_t* src = output.c_str();
		const wchar_t* srcEnd = src + output.size();
		wchar_t* dst = buffer;
		for ( ; src < srcEnd; ++src, ++dst )
		{
			*( (char*)dst ) = *( ((const char*)src) + 1 );
			*( ((char*)dst) + 1 ) = *( (const char*)src );
		}
		file.write( (char*)buffer, output.size() * 2 );
		delete[] buffer;
	}
	else if ( encode == UTF_8 || encode == UTF_8_NO_MARK )
	{
		if ( encode == UTF_8 )
		{
			bom[0] = 0xef;
			bom[1] = 0xbb;
			bom[2] = 0xbf;
			file.write( (char*)bom, 3 );
		}
		size_t tempBufferSize = output.size() * 4;	//up to 4 bytes per character
		char* tempBuffer = new char[tempBufferSize];
		size_t converted = utf16toutf8( output.c_str(), output.size(), tempBuffer, tempBufferSize );
		file.write( tempBuffer, converted );
		delete[] tempBuffer;
	}
#else
	file.write( output.c_str(), output.size() );
#endif

	file.close();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlDocument::parse( const Char* input, size_t size )
{
	const Char* cur = input;
	const Char* end = input + size;

	const Char* label = NULL;
	size_t labelSize = 0;
	int depth = 0;	
	XmlNode* currentNode = this;

	while ( cur < end )
	{
		assert( depth >= 0 );
		assert( currentNode != NULL );

		const Char* lastPos = cur;
		if ( !findLabel( cur, end - cur, label, labelSize ) )
		{
			break;
		}
		switch ( *label )
		{
		case T('/'):	//node ending
			if ( depth < 1 )
			{
				return false;
			}
			if ( currentNode->getType() == ELEMENT && !currentNode->hasChild() )
			{
				currentNode->assignString( lastPos, label - lastPos - 1 );
			}
			currentNode = currentNode->getParent();
			--depth;
			break;
		case T('?'):	//xml define node, ignore
			break;
		case T('!'):	//comment node
			{
				//ignore !-- and --
				if ( labelSize < 5 )
				{
					return false;
				}
				XmlNode* comment = currentNode->addChild( NULL, COMMENT );
				comment->assignName( label + 3, labelSize - 5 );
			}
			break;
		default:	//node start
			{
				XmlNode* newNode = currentNode->addChild( NULL, ELEMENT );

				parseLabel( newNode, label, labelSize );

				if ( *( label + labelSize - 1 ) != T('/') )
				{
					currentNode = newNode;
					++depth;
				}
			}
		}
	} // while( cur < end )

	if ( depth != 0 )
	{
		return false;
	}
	assert( currentNode == this );
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool XmlDocument::findLabel( const Char* &begin, size_t size, const Char* &label, size_t &labelSize )
{
	const Char* left = (const Char*)Memchr( begin, T('<'), size );
	if ( left == NULL )
	{
		return false;
	}
	label = left + 1;
	size -= ( label - begin );

	//comment is special, won't end without "-->"
	if ( size > 6 //Strlen( T("!---->") )
		&& label[0] == T('!')
		&& label[1] == T('-')
		&& label[2] == T('-') )
	{
		//buffer is not NULL-terminated, so we can't use strstr, shit! is there a "safe" version of strstr?
		const Char* cur = label + 3;	//skip !--
		size -= 5; //( Strlen( T("!---->") ) - 1 );
		while ( true )
		{
			const Char* end = (const Char*)Memchr( cur, T('-'), size );
			if ( end == NULL )
			{
				return false;
			}
			if ( *(end+1) == T('-') && *(end+2) == T('>' ) )
			{
				//get it
				labelSize = end - label + 2;
				begin = end + 3;
				return true;
			}
			size -= ( end - cur + 1 );
			cur = end + 1;
		}
	}
	const Char* right = (const Char*)Memchr( label, T('>'), size );
	if ( right == NULL )
	{
		return false;
	}
	labelSize = right - label;
	begin = right + 1;
	if ( labelSize == 0 )
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlDocument::parseLabel( XmlNode* node, const Char* label, size_t labelSize )
{
	//get name
	const Char* cur = label;
	while ( *cur != T(' ') && *cur != T('/') && *cur != T('>') )
	{
		++cur;
	}
	node->assignName( label, cur - label );

	if ( *cur == T(' ') )
	{
		//get attributes
		const Char* end = label + labelSize;
		
		while ( cur < end )
		{
			while ( *cur == T(' ') )
			{
				++cur;
			}
			//attribute name
			const Char* attriName = cur;
			while ( *cur != T(' ') && *cur != T('=') && *cur != T('/') && *cur != T('>') )
			{
				++cur;
			}
			size_t attriNameSize = cur - attriName; 

			//attribute value
			cur = (const Char*)Memchr( cur, T('"'), end - cur );
			if ( NULL == cur )
			{
				return;
			}
			const Char* attriValue = ++cur;
			cur = (const Char*)Memchr( cur, T('"'), end - cur );
			if ( NULL == cur )
			{
				return;
			}
			size_t attriValueSize = cur - attriValue;

			XmlAttribute* attribute = node->addAttribute();
			attribute->assignName( attriName, attriNameSize );
			attribute->assignString( attriValue, attriValueSize );
			++cur;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
size_t utf8toutf16( const char* u8, size_t size, wchar_t* u16, size_t outBufferSize )
{
	size_t converted = 0;

	while ( size > 0 )
	{
		if ( ( *u8 & 0x80 ) == 0 )
		{
			*(u16++) = *(u8++);
			--size;
			++converted;
		}
		else if ( ( *u8 & 0xe0 ) == 0xc0 )
		{
			if ( size < 2 )
			{
				break;
			}
			*(u16++) = ( *u8 & 0x1f ) | ( ( *(u8+1) & 0x3f ) << 5 );
			u8 += 2;
			size -= 2;
			++converted;
		}
		else if ( ( *u8 & 0xf0 ) == 0xe0 )
		{
			if ( size < 3 )
			{
				break;
			}
			*u16 = ( ( *u8 & 0x0f ) << 12 ) | ( ( *(u8+1) & 0x3f ) << 6 ) | ( *(u8+2) & 0x3f );
			u8 += 3;
			++u16;
			size -= 3;
			++converted;
		}
		else
		{
			break;
		}
		if ( converted == outBufferSize )
		{
			break;
		}
	}
	return converted;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
size_t utf16toutf8( const wchar_t* u16, size_t size, char* u8, size_t outBufferSize )
{
	size_t converted = 0;

	while ( size > 0 )
	{
		if ( *u16 < 0x80 )
		{
			//1 byte
			if ( converted == outBufferSize )
			{
				break;
			}
			*(u8++) = static_cast<char>( *(u16++) );
			--size;
			++converted;
		}
		else if ( *u16 < 0x800 )
		{
			//2 bytes
			if ( converted + 2 > outBufferSize )
			{
				break;
			}
			*u8 = ( *u16 >> 6 ) | 0xc0;
			*(u8+1) = ( *u16 & 0x3f ) | 0x80;
			u8 += 2;
			++u16;
			--size;
			converted += 2;
		}
		else
		{
			//3 bytes
			if ( converted + 3 > outBufferSize )
			{
				break;
			}
			*u8 = ( *u16 >> 12 ) | 0xe0;
			*(u8+1) = ( ( *u16 >> 6 ) & 0x3f ) | 0x80;
			*(u8+2) = ( *u16 & 0x3f ) | 0x80;
			u8 += 3;
			++u16;
			--size;
			converted += 3;
		}
	}
	return converted;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Encode detectEncode( const char* str, size_t size, bool& multiBytes )
{
	while ( size > 0 )
	{
		if ( ( *str & 0x80 ) == 0 )
		{
			//1 byte
			++str;
			--size;
		}
		else
		{
			multiBytes = true;
			if ( ( *str & 0xf0 ) == 0xe0 )
			{
				//3 bytes
				if ( size < 3 )
				{
					return ANSI;
				}
				if ( ( *(str+1) & 0xc0 ) != 0x80 || ( *(str+2) & 0xc0 ) != 0x80 )
				{
					return ANSI;
				}
				str += 3;
				size -= 3;
			}
			else if ( ( *str & 0xe0 ) == 0xc0 )
			{
				//2 bytes
				if ( size < 2 )
				{
					return ANSI;
				}
				if ( ( *(str+1) & 0xc0 ) != 0x80 )
				{
					return ANSI;
				}
				str += 2;
				size -= 2;
			}
			else if ( ( *str & 0xf8 ) == 0xf0 )
			{
				//4 bytes
				if ( size < 4 )
				{
					return ANSI;
				}
				if ( ( *(str+1) & 0xc0 ) != 0x80 || ( *(str+2) & 0xc0 ) != 0x80 || ( *(str+3) & 0xc0 ) != 0x80 )
				{
					return ANSI;
				}
				str += 4;
				size -= 4;
			}
			else
			{
				return ANSI;
			}
		}
	}
	return multiBytes ? UTF_8 : ANSI;
}

}
