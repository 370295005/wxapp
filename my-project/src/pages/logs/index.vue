<template>
  <div>
    <div class="Container">
      <div class="wrap">
        <mpvue-echarts :echarts="echarts" :onInit="onInit" />
      </div>
      <div class="data">12312312</div>
    </div>
  </div>
</template>

<script>
import * as echarts from "echarts/dist/echarts.simple.min";
import mpvueEcharts from "mpvue-echarts";
import { mapState } from "vuex";
function initChart(canvas, width, height) {
  const chart = echarts.init(canvas, null, {
    width: width,
    height: height,
  });
  canvas.setChart(chart);
  var option = {
    backgroundColor: "#fff",
    color: ["#37A2DA", "#67E0E3"],

    legend: {
      data: ["B"],
    },
    grid: {
      containLabel: true,
    },
    xAxis: {
      type: "category",
      data: ["周一", "周二", "周三", "周四", "周五", "周六", "周日"],
    },
    yAxis: {
      x: "center",
      type: "value",
      splitLine: {
        lineStyle: {
          type: "dotted",
        },
      },
    },
    series: [
      {
        name: "B",
        type: "line",
        smooth: true,
        data: this.day3,
      },
    ],
  };
  chart.setOption(option);
  return chart;
}
export default {
  data() {
    return {
      echarts,
      onInit: initChart,
    };
  },
  computed: {
    ...mapState({
      day3: (state) => state.analysis.day3,
    }),
  },
  components: {
    mpvueEcharts,
  },
  onShow() {
    console.log(this.day3);
    console.log(Array.isArray(this.day3));
  },
  onShareAppMessage() {},
};
</script>

<style scoped>
.Container {
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: space-between;
  box-sizing: border-box;
}
.wrap {
  width: 100%;
  height: 250px;
}
</style>