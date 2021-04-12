<template>
  <div class="container">
    <div class="wrap">
      <mpvue-echarts :echarts="echarts" :onInit="handleInit" ref="echarts" />
    </div>
  </div>
</template>
<script>
import { mapState } from "vuex";
import * as echarts from "echarts/dist/echarts.min";
import mpvueEcharts from "mpvue-echarts";
let chart = null;
export default {
  data() {
    return {
      option: null,
      echarts,
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
        color: ["#3d7ef9"],
        title: {
          text: "最高温度",
        },
        tooltip: {
          trigger: "axis",
        },
        grid: {
          containLabel: false,
          tooltip: {
            show: true,
            trigger: "axis",
          },
        },
        xAxis: {
          type: "category",
          name: "min",
          nameLocation: "end",
          data: this.currenttime,
          // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
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
        dataZoom: [
          // {
          //   start: 0,
          //   end: 20,
          //   type: "inside",
          //   filterMode: "filter",
          // },
          {
            start: 0,
            end: 50,
            type: "inside",
          },
        ],
        series: [
          {
            name: "温度",
            type: "line",
            smooth: false, //是否平滑显示曲线
            label: {
              //是否显示标签
              normal: {
                show: true,
              },
            },
            data: this.devicetempdata,
            // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
          },
        ],
      };
      this.$refs.echarts.init();
    },
    //初始化echarts
    handleInit(canvas, width, height) {
      chart = echarts.init(canvas, null, {
        width: width,
        height: height,
      });
      canvas.setChart(chart);
      chart.setOption(this.option);
      return chart;
    },
    //更新数据
    query() {
      const that = this;
      that.initChart();
    },
  },
};
</script>

<style scoped>
.wrap {
  width: 100%;
  height: 300px;
}
</style>