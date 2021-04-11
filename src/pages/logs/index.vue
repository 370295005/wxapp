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
    console.log(this.devicetempdata);
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
          show: true,
        },
        legend: {
          data: ["温度"],
          top: "top",
        },
        grid: {
          left: "4%", //设置Y轴name距离左边容器的位置,类似于margin-left
          right: "4%",
          bottom: "2%",
          containLabel: true,
        },
        xAxis: {
          type: "category",
          name: "min",
          nameLocation: "middle",
          data: this.currenttime,
          axisLabel: {
            interval: 0,
          },
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
        axisTick: {
          inside: true,
          lignWithLabel: true, //这两行代码可以让刻度正对刻度名
        },
        dataZoom: [
          // {
          //   //Y轴固定,让内容滚动
          //   type: "slider",
          //   show: false,
          //   xAxisIndex: [0],
          //   start: 1,
          //   end: 80, //设置X轴刻度之间的间隔(根据数据量来调整)
          //   zoomLock: true, //锁定区域禁止缩放(鼠标滚动会缩放,所以禁止)
          // },
          {
            type: "slider",
            start: 0,
            end: 50,
            zoomLock: true, //锁定区域禁止缩放
            show: true,
            xAxisIndex: [0],
            handleSize: 0, //滑动条的 左右2个滑动条的大小
            height: 10, //组件高度
            left: "10%", //左边的距离
            right: "10%", //右边的距离
            bottom: 26, //右边的距离
            borderColor: "#000",
            fillerColor: "#269cdb",
            borderRadius: 5,
            backgroundColor: "#33384b", //两边未选中的滑动条区域的颜色
            showDataShadow: false, //是否显示数据阴影 默认auto
            showDetail: false, //即拖拽时候是否显示详细数值信息 默认true
            realtime: true, //是否实时更新
            filterMode: "filter",
          },
        ],
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