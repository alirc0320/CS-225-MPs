/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource, vector<TileImage>& theTiles) {
    vector<Point<3>> all_points;
    map<Point<3>, TileImage*> tiles_map;
    for (auto& tile : theTiles) {
        LUVAPixel avg_color = tile.getAverageColor();
        Point<3> color_xyz = convertToXYZ(avg_color);
        all_points.push_back(color_xyz);
        tiles_map[color_xyz] = &tile;
    }

    KDTree<3> kdtree(all_points);
    MosaicCanvas* mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    for (int row = 0; row < mosaic->getRows(); ++row) {
        for (int col = 0; col < mosaic->getColumns(); ++col) {
            Point<3> target_color = convertToXYZ(theSource.getRegionColor(row, col));
            Point<3> nearest_color = kdtree.findNearestNeighbor(target_color);
            TileImage* nearest_tile = tiles_map[nearest_color];
            mosaic->setTile(row, col, nearest_tile);
        }
    }

    return mosaic;
}
//ALI