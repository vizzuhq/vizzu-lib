# Changelog

## [Unreleased]

### Added

- Axis line, labels, ticks, interlacing and guide can be switched on/off 
  via channel config parameters.

## [0.5.2] - 2022-08-29

### Fixed

- Marker guides switch off on polar scatterplots for performance purposes.
- Fixed unintentional size change of circle markers during polar-cartesian 
  coordinate system change and animation from/to treemap.
- Fixed line width animation when geometry is changing.
- Removed unwanted move around of marker linking first and last data point in
  polar coordinates during animation. 

## [0.5.1] - 2022-07-14

### Fixed

- Typescript declaration files fixed. 

## [0.5.0] - 2022-07-13

### Fixed

- Allow more than 5 colors in TS color palette and gradient declaration. 
  The format won`t be checked in compile time, only in runtime.
- animation-begin event called after actual animation is set up.
- Animation control methods take effect immediately.
- Wrong orientation after switching from circle geometry fixed.

### Added

- Presets introduced for specific chart types.
- cancel() method added for animation causing the animation to reset back to 
  the start position and rejecting the animation promise.
- Data series can be reset with new values, previously any attempt to set
  series with existing name resulted in error.
- Logging and rendering can be switched on/off through feature() method.
- position animation parameter for setting starting position of the animation.
- data property of the chart in JS API contains metadata about the data set.

## [0.4.8] - 2022-06-30

### Fixed

- styles property of Vizzu class on JS API fixed (renamed to style).

## [0.4.7] - 2022-03-18

### Fixed

- Runtime problem fixed for stacked charts.

## [0.4.6] - 2022-03-12

### Fixed

- Fixed Vizzu.options() function signature in .d.ts file.

## [0.4.5] - 2022-03-11

### Fixed

- Fixed canvas border clean on transparent background.

### Added

- URL of Wasm binary can be overriden using `Vizzu.options({ wasmUrl })`.

## [0.4.4] - 2022-03-10

### Fixed

- User friendly error when methods called on unitialized library.
- API throws exception instead of console log.

### Added

- Mouse event data contains mouse coordinates in plot coordinate system.
- Mouse wheel event introduced.

## [0.4.3] - 2022-01-31

### Fixed

- Transparent interlacing color prevented axis title draw - fixed.

### Added

- 3 hex character CSS color format support.
- Vizzu logo is a link to the lib's web page.

## [0.4.2] - 2021-12-22

### Fixed

- Fixed error on chart.animate({ style: null })

## [0.4.1] - 2021-12-22 - deprecated

## [0.4.0] - 2021-12-21

### Fixed

- Marker label fade-in/fade-out fixed, values are interpolated only if measure 
  is not changed on label channel.

### Added

- animate() method returns an animation control object, 
  animation control methods are chainable.
- CSS properties can be used to style vizzu charts
  E.g. `--vizzu-plot-marker-colorPalette: whatever` for `{style: {plot: {marker: {colorPalette: "whatever"}}}}`
- `maxFractionDigits` style parameter added to labels showing numbers.
- WASM build size reduction.

## [0.3.3] - 2021-10-17

### Fixed

- NPM package and d.ts file fixed for TypeScript projects.

## [0.3.2] - 2021-10-15

### Fixed

- Color range legend labels show min/max of color range instead of data min/max.
- Logo bottom padding fixed.

### Added

- Parts of markers outside of plot area are getting clipped. This behaviour can 
  be controlled by style.plot.overflow parameter.
- channel title parameter has "auto" value by default. "null" will switch the
  title off.

## [0.3.1] - 2021-09-24

### Fixed

- Data cube input recognition runtime error fixed.

## [0.3.0] - 2021-09-23

### Added

- First public release
