# Changelog

## [Unreleased]

### Fixed

- Implemented deep copying of received values to ensure the original data 
  remains unchanged during subsequent modifications.
- detach() won't fail on not initialized chart.
- detach() removes the canvas if it was created by the lib. 
- When the labelLevel was set to a value other than 0, the axis labels were 
  not displayed correctly.
- From now orientation is a persistent setting too.
- Order-independent channel input processing.
- Filtered out markers were taken into account for measure type, and waterfall
  chart dimension axis ranges.
- When measure set on color channel, the color changes alongside the color range
  instead of blending the starting and ending colors.
- Pointer event coordinates fixed when canvas is in CSS transformed view.
- Title appearance and disappearance glitch is fixed.
- Anim.Control.seek() won't fail when Number passed instead of string.

### Added

- Event objects are containing the element of the chart which the events were 
  triggered on.
- Drawing events are containing drawing geometry in an improved format.
- Missing TypeScript definitions for Event objects added.
- getCanvasElement() introduced to expose the underlying HTML canvas element.
- forceUpdate() introduced to trigger chart re-rendering.
- Converter functions to convert between canvas and relative coordinates.
- Orientation new default parameter: "auto".
- New dataseries 'unit' parameter introduced, undocumented unit parsing from 
  parameter name string removed.
- New parameters for the chart: subtitle and caption.

## [0.8.1] - 2023-08-15

### Fixed

- Unregistering was not working correctly when the same callback was added for
  multiple events. (#75)
- The plot.marker.label.color setting now works in a way that modifies the
  previously calculated color based on the transparency channel of the color
  setting. (#144)
- When rapidly clicking multiple times while the selection animation has not
  finished, the library is aborted due to an exception.
- At times, the chosen marker would disappear from the selections during
  rapid mouse actions, leading to an exception being triggered.
- Sometimes, during mouse actions, when the tooltip animation disappeared, the
  marker information was null. With a new mouse action, when other tooltip 
  appeared, the marker transformation went from null to null, which caused an
  exception.

## [0.8.0] - 2023-07-12

### Fixed

- Missing Area/line marker rewireing (on orientation change,
  base dimension change) added.
- Fixed line drawing for line segments partially outside of the plot.
- Fixed label position for marker in polar origo.
- Axis labels won't shift if their 'side' style changed but
  their 'position' style is also changing or they are fading out anyway.
- Axis labels won't fade in partially when fading out but their 'position'
  style is also changing.
- Axis titles fade-in/out when changing position instead of traveling through
  the chart.
- Axis titles are rotating with the axis during cartesian/polar coordinate
  system change.
- Fixed unintentional separate animation steps for specific dimension change
  animations.
- Fixed JS exception mishandling as C++ exception when thrown from webassembly.
- Fixed showing big numbers greater than 18446744073709551615.
- Fixed mouse events/selection on line-segment markers.
- Vertical polar area chart tangential borders "follow" the coordinate system
  instead of being forced to remain straight.

### Added

- New&renamed mouse events: pointeron, pointermove, pointerdown, pointerup
- New drawing events: draw-begin, draw-complete, plot-area-draw
- 'step' channel config parameter for set interlacing/tick/label intervals
  on axises.
- 'plot.areaColor' style parameter added.
- Marker click works mid-animation.
- Various aggregator functions for channels beside previous sole summing:
  'min()', 'max()', 'mean()', 'sum()', 'count()', 'distinct()'.
- New, fast bubble chart algorithm.
- Data records can be specified via objects beside arrays.
- Area/line rewiring animation (aka. connection) can be configured separately
  in animation options.
- WASM Binary size reduction.

## [0.7.2] - 2023-05-09

### Fixed

- Animation finishes also it begining reached at reverse play.
- Fixed garbage collection of stored charts and animations.
- If dimension data series re-added, previously existed categories and order
  will remains.

### Added

- New 'direction' anim option introduced.

## [0.7.1] - 2023-02-24

### Fixed

- Title animation timing on first chart fixed.

## [0.7.0] - 2023-02-23

### Fixed

- Fixed the UnPivot.convert method not modifying the original data object.
- Axis line, labels, ticks, interlacing and guide can be set to auto.
- Simple fade in case of empty target chart
- Fixed length serialization in style() for % unit.
- Fixed missing rendering update when duration is 0 in first animate call.
- Fixed error on multiple calls of the JS chart's 'data' property.
- Fixed disapearing title from empty charts.
- Fixed animation cancelling, did not trigger promise rejection.
- Fixed chart state reset on animation cancel. Used the target chart's config
  further on despite the cancellation.
- Fixed the type definition of Snapshot in the d.ts file.

### Added

- 'regroupStrategy' animation option introduced to control the algorithm for
  transitioning between charts having the data grouped differently on them
  (containing a different set of categorical dataseries).
- Multi keyframe animation support (one animation through multiple chart).
- 'style' property returns the style object only filled with the user-set
  values, all the values (returned by this property till 0.6.x) can be get
  using the new 'getComputedStyle()' method.
- detach() method added to JS API for enabling proper garbage collection.
- Actual animation can be stored for later reuse through Anim.Control.store()
  method.
- animate() returned promise is not an animation controller object from now on,
  but has a member promise called 'activated', which resolves to the controller.
- New style parameter, 'numberScale' has been introduced for setting the scale
  system for big numbers e.g.: K M B T or k m bn tn.
- Improved default options for animations: marker geometry, marker fade-in,
  marker position, coordinate system, title.

## [0.6.1] - 2022-11-22

### Fixed

- Treemap fixed for data series containing negative values.

### Added

- Transition (instead of fade) between chart showing different categorical
  dataseries.

## [0.6.0] - 2022-10-18

### Fixed

- Label side parameter animation fixed.
- Parsing color palette string containing space fixed.
- Fixed fontStyle change triggers animation now.
- Fixed label angle setting with deg/grad/turn units.
- Eliminated unnecessary turns in Label angle animation.
- Fixed animation section wise easing settings.
- Area/line marker label fade-in/out fixed.
- Rare missing marker on polar scatterplot fixed.
- Markers drawn even if data point is outside of the plot,
  if the marker intersects it.
- Fixed unwanted partial fade of non-changing legend when switched
  between auto and explicit value.
- Area/Line fade easing base made linear.
- Fixed missing last interlacing lane in negative chart areas

### Added

- Axis line, labels, ticks, interlacing and guide can be switched on/off
  via channel config parameters.
- Padding defaults changed.
- Marker labels added for some presets.

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
