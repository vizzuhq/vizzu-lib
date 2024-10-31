# Stacking explanation

In the following animation you can see an example how selecting different
measures and dimensions cause different stacking behaviour in the chart.

First we set the x channel to a dimension and an arbitrary constant value to
illustrate stacking. (The dimension divides the bar to equal sections along the
x-axis.) Then by changing the constant to a measure the markers’ width change
according to the values. Finally when the dimension is removed the bars get
ordered by the measure.

Note that when no dimension is present then the markers are measured from the
origo.

<div id="tutorial_01"></div>

<script src="../snippet.js" data="./stacking_explanation/data.js"></script>
