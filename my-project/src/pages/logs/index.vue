<template>
  <div class="container">
    <div class="wrap">
      <mpvue-echarts
        lazyLoad
        :echarts="echarts"
        :onInit="handleInit"
        ref="echarts"
      />
    </div>
  </div>
</template>

<script>
import { mapState } from "vuex";
import * as echarts from "echarts/dist/echarts.simple.min";
import mpvueEcharts from "mpvue-echarts";
let chart = null;
export default {
  data() {
    return {
      option: null,
      echarts,
      share: [],
    };
  },
  components: {
    mpvueEcharts,
  },
  computed: {
    ...mapState({
      devicetempdata: (state) => state.devicetempdata,
      currenttime: (state) => state.currenttime,
    }),
  },
  mounted() {
    this.query();
  },
  watch: {
    devicetempdata() {
      this.query();
    },
  },
  methods: {
    initChart() {
      this.option = {
        backgroundColor: "#fff",
        color: ["#67E0E3"],
        title: {
          text: "最高温度",
        },
        tooltip: {
          trigger: "axis",
        },
        legend: {
          data: ["温度"],
          top: "top",
        },
        grid: {
          containLabel: true,
        },
        xAxis: {
          type: "category",
          name: "min",
          nameLocation: "middle",
          data: this.currenttime,
          minInterval:1
        },
        yAxis: {
          type: "value",
          splitLine: {
            lineStyle: {
              type: "dashed",
            },
          },
          name: "Temp",
          nameLocation: "end",
        },
        series: [
          {
            name: "温度",
            type: "line",
            smooth: false,
            label: {
              normal: {
                show: true,
                padding: [0, 7, 0, 0],
              },
            },
            data: this.devicetempdata,
          },
        ],
        dataZoom: {
          start: 0,
          type: "inside",
          rangeMode: ["value", "value"],
        },
      };
      this.$refs.echarts.init();
    },
    handleInit(canvas, width, height) {
      chart = echarts.init(canvas, null, {
        width: width,
        height: height,
      });
      canvas.setChart(chart);
      chart.setOption(this.option);
      return chart;
    },
    query() {
      const that = this;
      that.initChart();
    },
  },
  onShareAppMessage() {},
};
</script>

<style scoped>
.wrap {
  width: 100%;
  height: 300px;
}
</style>