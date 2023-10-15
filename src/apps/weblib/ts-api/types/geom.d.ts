/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

export namespace Geom {
  /** Object representing a 2D point. */
  interface Point {
    /** X coordinate of the point. */
    x: number
    /** Y coordinate of the point. */
    y: number
  }
  /** Object representing a rectangle. */
  interface Rect {
    /** Position of the rectangle's top-left corner. */
    pos: Point
    /** Size of the rectangle. */
    size: Point
  }
  /** Object representing a line section. */
  interface Line {
    /** Starting point of the line. */
    begin: Point
    /** End point of the line. */
    end: Point
  }
  /** Object representing an affine transformation 2x3 matrix. */
  type AffineTransform = number[][]
  /** Rectangle transformed by an affine transformation. */
  interface TransformedRect {
    /** Transform transforms the top left corner to the origo. */
    transform: AffineTransform
    /** Size of the rectangle. */
    size: Point
  }
  /** Converts a point between two coordinate system. */
  type Converter = (point: Point) => Point
  /** Type of a point's coordinate system. */
  type CoordinateType = 'relative' | 'canvas'
}
