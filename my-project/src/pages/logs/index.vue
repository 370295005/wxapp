<template>
  <div class="container">
    <div class="wrap">
      <mpvue-echarts :echarts="echarts" :onInit="handleInit" ref="echarts" />
      <!-- <mpvue-echarts :echarts="echarts" :onInit="HandleInit" ref="Echarts" /> -->
    </div>
    <div class="analysis">
      <div>最高温度:{{ maxtemp }}°C</div>
      <div>最低温度:{{ mintemp }}°C</div>
      <div>最高湿度:{{ maxhum }}%</div>
      <div>最低湿度:{{ minhum }}%</div>
      <div>平均温度:{{ avgtemp }}°C</div>
      <div>平均湿度:{{ avghum }}%</div>
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
      maxtemp: 0,
      mintemp: 0,
      maxhum: 0,
      minhum: 0,
      avgtemp: 0,
      avghum: 0,
    };
  },
  components: {
    mpvueEcharts,
  },
  computed: {
    ...mapState({
      devicetempdata: (state) => state.devicetempdata,
      devicehumdata: (state) => state.devicehumdata,
      currenttime: (state) => state.currenttime,
    }),
  },
  mounted() {
    this.initChart();
    this.query();

  },
  watch: {
    devicetempdata() {
      this.query();
      this.maxtemp = Math.max.apply(null, this.devicetempdata);
      this.mintemp = Math.min.apply(null, this.devicetempdata);
      this.maxhum = Math.max.apply(null, this.devicehumdata);
      this.minhum = Math.min.apply(null, this.devicehumdata);
      this.avgtemp = this.avg(this.devicetempdata);
      this.avghum = this.avg(this.devicehumdata);
    },
  },
  methods: {
    initChart() {
      this.option = {
        backgroundColor: "#fff",
        // color: ["#3d7ef9", "#00FF7F"],
        title: {
          text: "设备数据统计",
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
        legend: {
          type: "plain",
          show: true,
          left: "center",
          top: "top",
        },
        xAxis: {
          type: "category",
          name: "time",
          nameLocation: "end",
          // data: this.currentsec + "分" + this.currentmin + '秒',
          data: this.currenttime,
          // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
        },
        yAxis: [
          {
            type: "value",
            splitLine: {
              lineStyle: {
                type: "dashed",
              },
            },
            name: "Temp",
            nameLocation: "end",
          },
        ],
        //x轴可滑动
        dataZoom: [
          {
            start: 50,
            end: 100,
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
          {
            name: "湿度",
            type: "line",
            smooth: false,
            label: {
              normal: {
                show: true,
              },
            },
            // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
            data: this.devicehumdata,
          },
        ],
      };
      this.$refs.echarts.init();
      // this.handleInit()
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
      this.initChart()
    },
    //数组求平均值
    avg(arr) {
      let len = arr.length;
      let sum = 0;
      for (let i = 0; i < len; i++) {
        sum += arr[i];
      }
      return parseFloat(sum / len).toFixed(2);
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