# Changelog

## [Unreleased]

### Fixed

- Do not interpolate hiding/showing legend

## [0.15.0] - 2024-10-28

### Fixed

- Markers are the same even if new record added.
- Flying out marker label fixed.
- Axis line hide/show at same time with axis labels/ticks/title.
- Do not draw invisible axis line.

### Changed

- Removed 'min' align property from the API which equivalent with the 'none'.
- Changed MarkerId to be a string instead of a number.

### Added

- Add marker top and center position to draw event.

## [0.14.0] - 2024-10-03

### Fixed 

- Fix again measure axis labels when the axis range and step set too.
- Fix drilldown regroup strategy on fake-split charts.
- From now vertical line connections are curved lines.
- Remove duplicated circles on line-circle transition.
- Fix area-circle polar connection transition.
- Fix line-rectangle polar connection linearity.
- Fix all polar connection interpolation (except fading).
- Remove unwanted line connections from line-circle + orientation changed anim.
- Move axis to the center on align: center charts.

### Changed

- Removed the 'align: max' property from the API. That function can be achieved 
  by setting the axis range to: {min: '100%', max: '0%'}.


## [0.13.0] - 2024-09-13

### Fixed

- Legend title bottomPadding extended.
- ColorGradient fromString not increasing position prohibited.
- Fix alpha settings to linear by default:
  - Axis: line, title, labels, guides, interlacing, ticks
  - Legend: title, dimension markers, measure extrema labels
  - Marker: line with connections
- Fix negative ranges on x, y, color (measure) and lightness.
- Fix axis step parameter if not match with the range sign (neg/pos).
- Fix axis interpolation. From now the axis and axis labels are following the markers.
- Fix measure axis labels when the range started after the 2000th step value from origo. 
- Remove marker labels at intermediate steps.
- Fix single value range interpolation and show.

### Added

- New style parameter for the legend scrolling.
- Remove background settings for all text, including title, subtitle and caption.

## [0.12.1] - 2024-08-22

### Fixed

- Area charts with data series started with zero: tooltip fixed.
- Series whose contained ',' and aggregated were not working properly.
- Dimension columns with non-existent values aggregation was undefined.

## [0.12.0] - 2024-07-29

### Fixed

- Make some static charts clearer:
  - Area/line charts different markers are not connected.
  - Not existing or disabled markers have no effect.
- Make some transactions clearer:
  - Rectangle - Line/Area first marker not fades, but shrinks.
  - The first marker's alpha was different to the other marker's alpha.
  - Marker connection rework: Introduce invalid, polar and self connection.
  - Marker connection animation step necessity and timing fixes.
  - Slipped animation steps (coordinateSystem - connection/orientation) fixes.
  - Filtered markers (and their connections):
    - Disappearing on hide animation phase.
    - Appearing on show animation phase.

### Added

- New data handling implemented:
  - Only existing data generates the chart.
  - Reduced memory usage.
  - Bigger data capacity.
- Canvas line drawing detail can be set (on C++ side).

## [0.11.4] - 2024-07-09

### Fixed 

- Add default no rectangle spacing on heatmap and coxcomb.
- Crash fix on special (meas-meas) area bar mekko charts.

## [0.11.3] - 2024-06-27

### Fixed

- Change bubblechart behavior on failure.
- Fix reference markers on bubblechart with negative values. 
- From now lightness channel goes from light (low value) to dark (high value) colors. 

## [0.11.2] - 2024-06-18

### Fixed

- Only dimension on size (+ color) wrongly displayed as treemap, not tablechart.
- Allocated memory is immediately freed after calling detach(), CAnimation and Snapshot
  also can be released using their new `free()` method.
- Canvas pointer event handlers registered as passive.

## [0.11.1] - 2024-05-31

### Fixed

- Remove hardly visible dimension axis guides between animation steps.
- Do not calculate disabled markers to the color normalization. 
- More precise TS type for transform matrix in event handlers. 
- Add missing canvas change function in htmlcanvas plugin. 
- On split charts the first range was not part of the separation calculation.
- When the first marker was disabled it was calculated as an enabled marker on the XY normalization.
- Sorted or reversed marker connections behaved chaotic at coordinate system change. 

## [0.11.0] - 2024-05-23

### Fixed

- next() can be called multiple times from Plugin hooks
- Fixed nan handling on non-axis charts.
- Line and circle chats with only dimensions on x, and y axes the markers were off the axis labels.
- Crash on TreeMap only with negative values
- On dimension axis where no marker, print the dimension name as default title.
- On measure axis with specified channel min-max where no marker, print the labels.
- Legend markers are not flickering on same data.
- From now legend appears/disappears linear time.
- Axis labels are not animated to a different axis label.
- Dimension axis density on sorted values was wrongly calculated.
- Tooltip with 'seriesName' does not rewrite first series data.
- Handle as different category the empty string and the missing value.
- On chart resize, the font size is recalculated. 
- Dim-Dim on x-y shows separated rectangles.

### Added

- New plugins and plugin hooks introduced: 
  - plugin: scheduler - plugin resposible for scheduling the rendering
  - plugin: clock - supplying the current time for the animation
  - plugin: canvasRenderer - plugin for rendering the chart on a htmlcanvas compatible canvas
  - hook: start - hook for starting the rendering loop
  - hook: render - hook for rendering the chart 
- Chart generation performance optimization: ~3x speed
- Maximum marker limit increased: ~2x count 
- Reduced wasm binary size: ~3%

## [0.10.1] - 2024-03-12

### Added

- Updated `type-fest` dependency version to `4.12.0`

## [0.10.0] - 2024-03-11

### Fixed

- Json serializer control character escape fixed. Some unicode characters
  were not escaped properly.
- Fix dimension label transition on axis and legend.
- Through event handler call, when a new event handler is registered, undefined behaviour happened.
- Fixed channel reset with empty array when shorthands plugin switched off.
- Legend label outerRect was not properly calculated.
- Line chart connector circles color was not contained the alpha channel.
- Line chart draws was overwrite the event's settings.
- Legend title outerRect was not properly calculated.
- Fixed stacked empty min/max aggregated values.
- Fixed error when an animation triggered during tooptip activation which removed the corresopnding marker. 

### Added

- In config channels, data series and their aggregators can be specified separately 
  in a descriptor object, besides encoding them into one string (old way).
- Added optional `categories` member to the `legend-marker`, `legend-label` and `plot-axis-label` events.
- Remove unused marker selection and selected marker coloring.
- Removed marker's alpha color when tooltip is shown.  
- Remove cursor modification over logo.
- Make `channel.step` option to work on dimensions.
- When X axis dimension labels are close to each other, they are rotated to avoid overlapping.
- The event handler registration order changed. Now the handlers are called in the opposite order of the registration.
- Added the padded rectangle, the bounding rectangle and the align parameter to the draw text event object.
- Tooltip works on marker labels too.

## [0.9.3] - 2023-12-20

### Fixed

- Orientation and channel min/max cannot be null, only auto
- Corrected listing of Type-fest dependency from development to normal.
- Added missing `cvizzu.d.ts` and `cvizzu.types.d.ts` to the npm package.
- Logo positioning issue in charts when only the logo is rendered and the chart is resized.
- API issue where exact chart position was not accurately queried during mouse events.
- Missing space between value and unit in axis labels.
- Bug fixed where rendered label units disappeared when target plot did not contain a unit.
- Configuration of experimental plot rotation feature changed from relative `rotate` to absolute `angle`. 

### Added

- Units on legend.

## [0.9.2] - 2023-11-22

### Fixed

- Fixed redraw on browser zoom (bug since 0.9.0).
- Fixed clearing of the canvas before rendering when devicePixelRatio is not 1.
- TypeScript definition fixed, measures can contain null values. 

### Added

- Animation control object expose properties `position`, `direction`, `speed` 
  and `playState` to be able to query the current state of the animation, and
  to be more conform with the Web Animation API. 

## [0.9.1] - 2023-11-15

### Fixed

- Fixed crash on splitted chart with no measure.
- Fixed refresh of chart when duration is 0.

## [0.9.0] - 2023-11-10

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
- When animation canceled, promise rejected with proper custom error. 
- No redraw on every mouse event.
- Anim options applies for the whole animation even when dimension changes
  cause multi-step animations.
- Animation between two version of the same chart is instant only if duration
  is not explicitly set
- Aggregators didn't work when there are parentheses in the name of the data 
  series
- Layout stretched to the bottom of the canvas. 
- Legend accepts only the acceptable channel values.
- From now the dimension axis title is shown if set.
- "step" was not listed in chart channel accessors.

### Added

- Event objects are containing the element of the chart which the events were 
  triggered on.
- Drawing events are containing drawing geometry in an improved format.
- Missing TypeScript definitions for Event objects added.
- Orientation new default parameter: `auto`.
- New dataseries `unit` parameter introduced, undocumented unit parsing from 
  parameter name string removed.
- New parameters for the chart: `subtitle` and `caption`.
- New `speed` parameter introduced for animation control object for setting 
  the speed of the animation.
- Wasm module gets loaded only once, not per chart. Load can be triggered optionally
  without any chart calling the `Vizzu.initialize()` method, otherwise will be 
  initialized automatically on first chart creation.
- Plugin interface introduced, new plugins can be registed through `Vizzu.feature()`.
- CSS parameter usage feature is disabled by default and can be enabled through
  `Vizzu.feature()` with name `cssProperties`.
- `shorthands`, `pointerEvents`, `pivotData`, `rendering` features can be disabled through
  `Vizzu.feature()`
- TypeScript interface made more typesafe, while the javascript api got ported to TypeScript.
- `pointeron` event removed, `pointerleave` event added. 
- `Vizzu.feature.htmlCanvas.element` introduced to expose the underlying HTML canvas element.
- `Vizzu.feature.rendering.update()` introduced to trigger chart re-rendering.
- `Vizzu.feature.coordSystem.toCanvas()`/`toRelative()` converter functions to convert
  between canvas and relative coordinates.


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
