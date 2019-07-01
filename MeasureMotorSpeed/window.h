/*
 * Copyright (c) 2008-2011 Zhang Ming (M. Zhang), zmjerry@163.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 2 or any later version.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. A copy of the GNU General Public License is available at:
 * http://www.fsf.org/licensing/licenses
 */


/*****************************************************************************
 *                                 window.h
 *
 * This file includes seven usually used windows in signal processing:
 *      Rectangle        Bartlett        Hanning     Hamming
 *      Blackman         Kaiser          Gauss
 * All of them are same to those in "Matlab".
 *
 * Zhang Ming, 2010-01, Xi'an Jiaotong University.
 *****************************************************************************/
#pragma once

#ifndef WINDOW_H
#define WINDOW_H


#include "vector.h"


namespace splab
{

    template<typename Type> Vector<Type> window( const string&, int, Type );
	template<typename Type> Vector<Type> window( const string&, int,
                                                 Type, Type );

    template<typename Type> Vector<Type> rectangle( int, Type );
    template<typename Type> Vector<Type> bartlett( int, Type );
    template<typename Type> Vector<Type> hanning( int, Type, int, double);
    template<typename Type> Vector<Type> hamming( int, Type, int, double);
    template<typename Type> Vector<Type> blackman( int, Type );
    template<typename Type> Vector<Type> kaiser( int, Type, Type );
    template<typename Type> Vector<Type> gauss( int, Type, Type );

	template<typename Type> Type I0( Type alpha );


	#include "window-impl.h"

}
// namespace splab


#endif
// WINDOW_H
