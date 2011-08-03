/*
 *  Red Bull Media Player
 *  Copyright (C) 2011, Red Bull
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TRANSFORMPARAMS_H
#define TRANSFORMPARAMS_H

class TransformParams {
    public:
        TransformParams( int w, int fh, int bh, int trdiff, int yPos ) {
            transformedWidth = w;
            frontHeight = fh;
            backHeight = bh;
            topRightDiff = trdiff;
            y = yPos;
        }

        int transformedWidth;
        int frontHeight;
        int backHeight;
        int topRightDiff;
        int y;
};

#endif // TRANSFORMPARAMS_H
