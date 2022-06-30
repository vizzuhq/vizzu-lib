export default class Presets {
  constructor(chart) {
    this._presetConfigs = {
      column: {
        channels: {
          x: "by",
          y: "of",
          label: "of",
        },
      },
      groupedColumn: {
        channels: {
          x: ["groupedBy", "by"],
          y: "of",
          label: "of",
          color: "by",
        },
      },
      stackedColumn: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          label: "of",
          color: "stackedBy",
        },
      },
      splittedColumn: {
        channels: {
          x: "by",
          y: ["of", "splittedBy"],
          color: "splittedBy",
        },
        split: true,
      },
      percentageColumn: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          color: "stackedBy",
        },
        align: "stretch",
      },
      waterfall: {
        channels: {
          x: "by",
          y: ["of", "by"],
          label: "of",
          color: "of",
        },
        align: "stretch",
      },
      mekko: {
        channels: {
          x: ["vs", "by"],
          y: ["of", "stackedBy"],
          color: "stackedBy",
          label: "by",
        },
      },
      marimekko: {
        channels: {
          x: ["vs", "by"],
          y: ["of", "stackedBy"],
          color: "stackedBy",
          label: "by",
        },
        align: "stretch",
        orientation: "horizontal",
      },
      bar: {
        channels: {
          x: "of",
          y: "by",
          label: "of",
        },
      },
      groupedBar: {
        channels: {
          x: "of",
          y: ["groupedBy", "by"],
          label: "of",
          color: "by",
        },
      },
      stackedBar: {
        channels: {
          x: ["of", "stackedBy"],
          y: "by",
          label: "of",
          color: "stackedBy",
        },
      },
      splittedBar: {
        channels: {
          x: ["of", "splittedBy"],
          y: "by",
          color: "splittedBy",
        },
        split: true,
      },
      percentageBar: {
        channels: {
          x: ["of", "stackedBy"],
          y: "by",
          color: "stackedBy",
        },
        align: "stretch",
      },
      lollipop: {
        channels: {
          x: "by",
          y: "of",
        },
        geometry: "circle",
      },
      scatter: {
        channels: {
          x: "of",
          y: "vs",
          noop: "by",
        },
        geometry: "circle",
      },
      bubbleplot: {
        channels: {
          x: "of",
          y: "vs",
          color: "by",
          size: "sizedBy",
        },
        geometry: "circle",
      },
      area: {
        channels: {
          x: "by",
          y: "of",
        },
        geometry: "area",
      },
      stackedArea: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          color: "stackedBy",
        },
        geometry: "area",
      },
      precentageArea: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          color: "stackedBy",
        },
        align: "stretch",
        geometry: "area",
      },
      splittedArea: {
        channels: {
          x: "by",
          y: ["of", "splittedBy"],
          color: "splittedBy",
        },
        split: true,
        geometry: "area",
      },
      stream: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          color: "stackedBy",
        },
        geometry: "area",
        align: "center",
      },
      verticalStream: {
        channels: {
          x: ["of", "stackedBy"],
          y: "by",
          color: "stackedBy",
        },
        geometry: "area",
        align: "center",
      },
      violin: {
        channels: {
          x: "by",
          y: ["of", "splittedBy"],
          color: "splittedBy",
        },
        geometry: "area",
        align: "center",
        split: true,
      },
      verticalViolin: {
        channels: {
          x: ["of", "splittedBy"],
          y: "by",
          color: "splittedBy",
        },
        geometry: "area",
        align: "center",
        split: true,
      },
      line: {
        channels: {
          x: "by",
          y: "of",
          color: "dividedBy",
        },
        geometry: "line",
      },
      verticalLine: {
        channels: {
          x: "of",
          y: "by",
          color: "dividedBy",
        },
        geometry: "line",
      },
      pie: {
        channels: {
          x: ["of", "by"],
          color: "by",
        },
        coordSystem: "polar",
      },
      polarColumn: {
        channels: {
          x: "by",
          y: "of",
        },
        coordSystem: "polar",
      },
      polarStackedColumn: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          color: "stackedBy",
        },
        coordSystem: "polar",
      },
      variableRadiusPie: {
        channels: {
          x: ["of", "by"],
          y: "vs",
          color: "by",
        },
        coordSystem: "polar",
      },
      polarArea: {
        channels: {
          x: "by",
          y: ["of", "stackedBy"],
          color: "stackedBy",
        },
        coordSystem: "polar",
        geometry: "area",
      },
      radialBar: {
        channels: {
          x: "of",
          y: "by",
        },
        coordSystem: "polar",
      },
      radialStackedBar: {
        channels: {
          x: ["of", "stackedBy"],
          y: "by",
          color: "stackedBy",
        },
        coordSystem: "polar",
      },
      donut: {
        channels: {
          x: ["of", "by"],
          color: "by",
        },
        coordSystem: "polar",
      },
      nestedDonut: {
        channels: {
          x: ["of", "by"],
          y: "stackedBy",
          color: "by",
          label: "of",
        },
        coordSystem: "polar",
        align: "stretch",
      },
      polarScatter: {
        channels: {
          x: "of",
          y: "vs",
          noop: "by",
        },
        coordSystem: "polar",
      },
      polarLine: {
        channels: {
          x: "in",
          y: "of",
          color: "by",
        },
        coordSystem: "polar",
        geometry: "line",
      },
      treemap: {
        channels: {
          size: ["of", "by"],
          color: "by",
          label: "by",
        },
      },
      stackedTreemap: {
        channels: {
          size: ["of", "stackedBy"],
          color: "by",
          label: "stackedBy",
          lightness: "stackedBy",
        },
      },
      heatmap: {
        channels: {
          x: "by",
          y: "vs",
          lightness: "of",
        },
      },
      bubble: {
        channels: {
          size: "of",
          color: "by",
          label: "by",
        },
        geometry: "circle",
      },
      stackedBubble: {
        channels: {
          size: ["of", "stackedBy"],
          color: "by",
        },
        geometry: "circle",
      },
    };

    for (let key in this._presetConfigs) {
      this[key] = (config) => {
        return this._buildPresetConfig(key, config);
      };
    }
  }

  _nullConfig() {
    return {
      align: "none",
      coordSystem: "cartesian",
      orientation: "horizontal",
      rotate: 0,
      split: false,
      channels: {
        x: null,
        y: null,
        color: null,
        lightness: null,
        size: null,
        noop: null,
        label: null,
      },
      legend: "auto",
      title: "",
      reverse: false,
      sort: "none",
    };
  }

  _createPresetConfig(presetName) {
    let presetConfig = this._presetConfigs[presetName];
    let base = Object.assign(this._nullConfig(), presetConfig);
    return base;
  }

  _getChannelCopy(channel) {
    if (Array.isArray(channel)) {
      return channel.map((v) => v);
    } else {
      return [channel];
    }
  }

  _fillChannels(presetConfig, config) {
    if (!config) return;
    let channels = presetConfig.channels;
    for (let channel in channels) {
      if (typeof channels[channel] === "string") {
        channels[channel] = this._getChannelCopy(config[channels[channel]]);
      } else if (Array.isArray(channels[channel])) {
        channels[channel] = channels[channel]
          .map((v) => this._getChannelCopy(config[v]))
          .flat();
      }
    }
  }

  _setupUserParams(base, config) {
    ["legend", "title", "reverse", "sort"].forEach((key) => {
      if (config[key] !== undefined) {
        base[key] = config[key];
      }
    });
  }

  _buildPresetConfig(presetName, config) {
    let presetConfig = this._createPresetConfig(presetName);
    this._fillChannels(presetConfig, config);
    this._setupUserParams(presetConfig, config);
    return presetConfig;
  }
}
