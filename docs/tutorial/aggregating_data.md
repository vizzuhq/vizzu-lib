---
data_url: ../../assets/data/music_data.js
---

# Aggregating data

The default logic of `Vizzu` is to show the sum of values that are in the
categories added to a chart. So if we create a simple vertical bar chart by
adding the `Popularity` measure to the y-axis, the height of the bar will be the
sum of all `Popularity` values and when we add `Genres` to the x-axis, the
height of the bars will be the sum of `Popularity` values in each category
within `Genres`.

<div id="tutorial_01"></div>

// {% include-markdown "tutorial/assets/setup/setup_a.md" %}

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['Popularity']
            }
        }
    }
})

chart.animate({
    config: {
        channels: {
            x: {
                set: ['Genres']
            }
        }
    }
})
```

Next to the default logic of sum, there are a handful of other aggregation
logics that are available in `Vizzu`: `min`, `max`, `mean`, `count` and
`distinct`. Aggregators can be set for data series using:

- data series descriptor objects: `{ name: 'Popularity', aggregator: 'min' }`
- or by encoding them into the name of the data series: `'min(Popularity)'`

We will use the second method in the following examples. Let's go through them
to see how they work.

Minimum value: the height of the bars show the minimum value in the `Popularity`
measure in each of the `Genres`.

<div id="tutorial_02"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['min(Popularity)']
            }
        }
    }
})
```

Maximum value: the height of the bars show the maximum value in the `Popularity`
measure in each of the `Genres`.

<div id="tutorial_03"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['max(Popularity)']
            }
        }
    }
})
```

Mean value: the height of the bars show the mean value of the `Popularity`
measure in each of the `Genres`.

<div id="tutorial_04"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['mean(Popularity)']
            }
        }
    }
})
```

Count: the height of the bars show the number of items (rows if you will) in
each of the `Genres`.

<div id="tutorial_05"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['count()']
            }
        }
    }
})
```

Distinct: the height of the bars show the number of distinct categories of
`Kinds` in each of the `Genres`.

!!! note
    Distinct aggregation logic relates to dimensions like `Genres` and not to
    measures like `Popularity`.

<div id="tutorial_06"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['distinct(Kinds)']
            }
        }
    }
})
```

Sum: this is how you can get back to the default aggregation logic of `Vizzu`
that sums the `Popularity` values in each of the `Genres`.

<div id="tutorial_07"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                set: ['sum(Popularity)']
            }
        }
    }
})
```

<script src="../aggregating_data.js"></script>
