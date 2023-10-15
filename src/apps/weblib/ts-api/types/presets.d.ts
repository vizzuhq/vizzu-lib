/* eslint-disable @typescript-eslint/no-unused-vars, lines-between-class-members, no-use-before-define */

import { Config } from './config'

export namespace Presets {
  interface Preset {
    legend?: 'color' | 'lightness' | 'size' | null
    title?: string | null
    subtitle?: string | null
    caption?: string | null
    reverse?: boolean
    sort?: 'none' | 'byValue'
  }
  interface Column extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface GroupedColumn extends Preset {
    groupedBy: string[] | string
    x: string[] | string
    y: string[] | string
  }
  interface StackedColumn extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface SplittedColumn extends Preset {
    x: string[] | string
    y: string[] | string
    splittedBy: string[] | string
  }
  interface PercentageColumn extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface Waterfall extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface Mekko extends Preset {
    x: string[] | string
    groupedBy: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface Marimekko extends Preset {
    x: string[] | string
    groupedBy: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface Bar extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface GroupedBar extends Preset {
    x: string[] | string
    groupedBy: string[] | string
    y: string[] | string
  }
  interface StackedBar extends Preset {
    x: string[] | string
    stackedBy: string[] | string
    y: string[] | string
  }
  interface SplittedBar extends Preset {
    x: string[] | string
    splittedBy: string[] | string
    y: string[] | string
  }
  interface PercentageBar extends Preset {
    x: string[] | string
    stackedBy: string[] | string
    y: string[] | string
  }
  interface Lollipop extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface Scatter extends Preset {
    x: string[] | string
    y: string[] | string
    dividedBy: string[] | string
  }
  interface Bubbleplot extends Preset {
    x: string[] | string
    y: string[] | string
    color: string[] | string
    size: string[] | string
    dividedBy: string[] | string
  }
  interface Area extends Preset {
    x: string[] | string
    y: string[] | string
  }
  interface StackedArea extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface PercentageArea extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface SplittedArea extends Preset {
    x: string[] | string
    y: string[] | string
    splittedBy: string[] | string
  }
  interface Stream extends Preset {
    x: string[] | string
    y: string[] | string
    stackedBy: string[] | string
  }
  interface VerticalStream extends Preset {
    x: string[] | string
    stackedBy: string[] | string
    y: string[] | string
  }
  interface Violin extends Preset {
    x: string[] | string
    y: string[] | string
    splittedBy: string[] | string
  }
  interface VerticalViolin extends Preset {
    x: string[] | string
    splittedBy: string[] | string
    y: string[] | string
  }
  interface Line extends Preset {
    x: string[] | string
    y: string[] | string
    dividedBy: string[] | string
  }
  interface VerticalLine extends Preset {
    x: string[] | string
    y: string[] | string
    dividedBy: string[] | string
  }
  interface Pie extends Preset {
    angle: string[] | string
    by: string[] | string
  }
  interface PolarColumn extends Preset {
    angle: string[] | string
    radius: string[] | string
  }
  interface PolarStackedColumn extends Preset {
    angle: string[] | string
    radius: string[] | string
    stackedBy: string[] | string
  }
  interface VariableRadiusPie extends Preset {
    angle: string[] | string
    by: string[] | string
    radius: string[] | string
  }
  interface RadialBar extends Preset {
    angle: string[] | string
    radius: string[] | string
  }
  interface RadialStackedBar extends Preset {
    angle: string[] | string
    stackedBy: string[] | string
    radius: string[] | string
  }
  interface Donut extends Preset {
    angle: string[] | string
    stackedBy: string[] | string
  }
  interface NestedDonut extends Preset {
    angle: string[] | string
    stackedBy: string[] | string
    radius: string[] | string
  }
  interface PolarScatter extends Preset {
    angle: string[] | string
    radius: string[] | string
    dividedBy: string[] | string
  }
  interface PolarLine extends Preset {
    angle: string[] | string
    radius: string[] | string
    dividedBy: string[] | string
  }
  interface Treemap extends Preset {
    size: string[] | string
    color: string[] | string
  }
  interface StackedTreemap extends Preset {
    size: string[] | string
    dividedBy: string[] | string
    color: string[] | string
  }
  interface Heatmap extends Preset {
    x: string[] | string
    y: string[] | string
    lightness: string[] | string
  }
  interface Bubble extends Preset {
    size: string[] | string
    color: string[] | string
  }
  interface StackedBubble extends Preset {
    size: string[] | string
    stackedBy: string[] | string
    color: string[] | string
  }
  /** Collection of factory functions for creating preset chart configs. */
  interface Presets {
    column(config: Column): Config.Chart
    groupedColumn(config: GroupedColumn): Config.Chart
    stackedColumn(config: StackedColumn): Config.Chart
    splittedColumn(config: SplittedColumn): Config.Chart
    percentageColumn(config: PercentageColumn): Config.Chart
    waterfall(config: Waterfall): Config.Chart
    mekko(config: Mekko): Config.Chart
    marimekko(config: Marimekko): Config.Chart
    bar(config: Bar): Config.Chart
    groupedBar(config: GroupedBar): Config.Chart
    stackedBar(config: StackedBar): Config.Chart
    splittedBar(config: SplittedBar): Config.Chart
    percentageBar(config: PercentageBar): Config.Chart
    lollipop(config: Lollipop): Config.Chart
    scatter(config: Scatter): Config.Chart
    bubbleplot(config: Bubbleplot): Config.Chart
    area(config: Area): Config.Chart
    stackedArea(config: StackedArea): Config.Chart
    percentageArea(config: PercentageArea): Config.Chart
    splittedArea(config: SplittedArea): Config.Chart
    stream(config: Stream): Config.Chart
    verticalStream(config: VerticalStream): Config.Chart
    violin(config: Violin): Config.Chart
    verticalViolin(config: VerticalViolin): Config.Chart
    line(config: Line): Config.Chart
    verticalLine(config: VerticalLine): Config.Chart
    pie(config: Pie): Config.Chart
    polarColumn(config: PolarColumn): Config.Chart
    polarStackedColumn(config: PolarStackedColumn): Config.Chart
    variableRadiusPie(config: VariableRadiusPie): Config.Chart
    radialBar(config: RadialBar): Config.Chart
    radialStackedBar(config: RadialStackedBar): Config.Chart
    donut(config: Donut): Config.Chart
    nestedDonut(config: NestedDonut): Config.Chart
    polarScatter(config: PolarScatter): Config.Chart
    polarLine(config: PolarLine): Config.Chart
    treemap(config: Treemap): Config.Chart
    stackedTreemap(config: StackedTreemap): Config.Chart
    heatmap(config: Heatmap): Config.Chart
    bubble(config: Bubble): Config.Chart
    stackedBubble(config: StackedBubble): Config.Chart
  }
}
