/*
OZCollide - Collision Detection Library
Copyright (C) 2006  Igor Kravtchenko

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact the author: igor@tsarevitch.org
*/

#include <ozcollide/ozcollide.h>

#ifndef OZCOLLIDE_PCH
#include <ozcollide/polygon.h>
#endif

ENTER_NAMESPACE_OZCOLLIDE

Polygon::Polygon()
{
	indices_ = NULL;
	nbIndices_ = 0;
}

Polygon::~Polygon()
{
	if (!(nbIndices_ & 0x100))
		SAFE_FREE(indices_);
}

void Polygon::setNbIndices(int _nbIndices)
{
	int *indices = (int*) malloc(_nbIndices * 4);
	if (indices_) {
		int size = _nbIndices > nbIndices_ ? nbIndices_ : _nbIndices;
		memcpy(indices, indices_, size * sizeof(int));
	}

	this->~Polygon();
	indices_ = indices;
	nbIndices_ = _nbIndices;
}

bool Polygon::isDegenerate() const
{
	return false;
}

void Polygon::copyTo(Polygon &_pol) const
{
	_pol.~Polygon();
	_pol.nbIndices_ = nbIndices_;
	_pol.normal_ = normal_;
	_pol.indices_ = new int[nbIndices_];
	memcpy(_pol.indices_, indices_, sizeof(int) * nbIndices_);
}

Polygon* Polygon::clone() const
{
	Polygon *pol = new Polygon();
	copyTo(*pol);
	return pol;
}

LEAVE_NAMESPACE
