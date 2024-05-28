/** Object representing a 2D point. */
export interface Point {
	/** X coordinate of the point. */
	x: number
	/** Y coordinate of the point. */
	y: number
}

/** Object representing a rectangle. */
export interface Rect {
	/** Position of the rectangle's top-left corner. */
	pos: Point
	/** Size of the rectangle. */
	size: Point
}

/** Object representing a line section. */
export interface Line {
	/** Starting point of the line. */
	begin: Point
	/** End point of the line. */
	end: Point
}

/** Object representing an affine transformation 2x3 matrix. */
export type AffineTransform = [[number, number, number], [number, number, number]]

/** Rectangle transformed by an affine transformation. */
export interface TransformedRect {
	/** Transform transforms the top left corner to the origo. */
	transform: AffineTransform
	/** Size of the rectangle. */
	size: Point
}

/** Converts a point between two coordinate system. */
export type PointConverter = (point: Point) => Point

/** Type of a point's coordinate system. */
export type CoordinateType = 'relative' | 'canvas'
