<template>
  <div class="Container">
    <div class="wrap">
      <mpvue-echarts
        :echarts="echarts"
        :onInit="handleInit"
        ref="echarts"
        canvasId="Temp"
      />
      <div class="analysis">
        <div>最高湿度:{{ maxhum }}%</div>
        <div>最低湿度:{{ minhum }}%</div>
        <div>平均湿度:{{ avghum }}%</div>
      </div>
      <mpvue-echarts
        :echarts="echarts"
        :onInit="HandleInit"
        ref="Echarts"
        canvasId="Hum"
      />
      <div class="analysis">
        <div>最高温度:{{ maxtemp }}°C</div>
        <div>最低温度:{{ mintemp }}°C</div>
        <div>平均温度:{{ avgtemp }}°C</div>
      </div>
    </div>
  </div>
</template>
<script>
import { mapState } from 'vuex'
import * as echarts from 'echarts/dist/echarts.min'
import mpvueEcharts from 'mpvue-echarts'
let chart = null
let Chart = null
export default {
  data () {
    return {
      option: null,
      Option: null,
      echarts,
      maxtemp: 0,
      mintemp: 0,
      maxhum: 0,
      minhum: 0,
      avgtemp: 0,
      avghum: 0
    }
  },
  components: {
    mpvueEcharts
  },
  computed: {
    ...mapState({
      devicetempdata: (state) => state.devicetempdata,
      devicehumdata: (state) => state.devicehumdata,
      currenttime: (state) => state.currenttime
    })
  },
  mounted () {
    this.Init()
  },
  watch: {
    devicetempdata () {
      this.updateData()
    },
    devicehumdata () {
      this.updateData()
    }
  },
  methods: {
    // 温度统计
    initChart () {
      this.option = {
        backgroundColor: '#fff',
        color: ['#ff3333'],
        title: {
          text: '温度统计'
        },
        tooltip: {
          trigger: 'axis'
        },
        grid: {
          containLabel: false,
          tooltip: {
            show: true,
            trigger: 'axis'
          }
        },
        legend: {
          type: 'plain',
          show: true,
          left: 'center',
          top: 'top'
        },
        xAxis: {
          type: 'category',
          name: 'time',
          nameLocation: 'end',
          nameGap: 5,
          // data: this.currentsec + "分" + this.currentmin + '秒',
          data: this.currenttime
          // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
        },
        yAxis: [
          {
            type: 'value',
            splitLine: {
              lineStyle: {
                type: 'dashed'
              }
            },
            name: 'Temp',
            nameLocation: 'end'
          }
        ],
        // x轴可滑动
        dataZoom: [
          {
            start: 50,
            end: 100,
            type: 'inside'
          }
        ],
        series: [
          {
            name: '温度',
            type: 'line',
            smooth: false, // 是否平滑显示曲线
            label: {
              // 是否显示标签
              normal: {
                show: true
              }
            },
            data: this.devicetempdata
            // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
          }
        ]
      }
      this.$refs.echarts.init()
    },
    // 湿度统计
    initchart () {
      this.Option = {
        backgroundColor: '#fff',
        color: ['#3d7ef9'],
        title: {
          text: '湿度统计'
        },
        tooltip: {
          trigger: 'axis'
        },
        grid: {
          containLabel: false,
          tooltip: {
            show: true,
            trigger: 'axis'
          }
        },
        legend: {
          type: 'plain',
          show: true,
          left: 'center',
          top: 'top'
        },
        xAxis: {
          type: 'category',
          name: 'time',
          nameLocation: 'end',
          nameGap: 5,
          data: this.currenttime
          // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
        },
        yAxis: [
          {
            type: 'value',
            splitLine: {
              lineStyle: {
                type: 'dashed'
              }
            },
            name: 'Hum',
            nameLocation: 'end'
          }
        ],
        // x轴可滑动
        dataZoom: [
          {
            start: 50,
            end: 100,
            type: 'inside'
          }
        ],
        series: [
          {
            name: '湿度',
            type: 'line',
            smooth: false,
            label: {
              normal: {
                show: true
              }
            },
            // data: [24, 25, 19, 29, 25, 28, 20, 25, 26, 22, 21, 25, 29],
            data: this.devicehumdata
          }
        ]
      }
      this.$refs.Echarts.init()
    },
    // 初始化echarts
    handleInit (canvas, width, height) {
      chart = echarts.init(canvas, null, {
        width: width,
        height: height
      })
      canvas.setChart(chart)
      chart.setOption(this.option)
      return chart
    },
    HandleInit (canvas, width, height) {
      Chart = echarts.init(canvas, null, {
        width: width,
        height: height
      })
      canvas.setChart(Chart)
      Chart.setOption(this.Option)
      return Chart
    },
    // 数组求平均值
    avg (arr) {
      let len = arr.length
      let sum = 0
      for (let i = 0; i < len; i++) {
        sum += arr[i]
      }
      return parseFloat(sum / len).toFixed(2)
    },
    // 更新数据
    updateData () {
      chart.setOption(this.option)
      Chart.setOption(this.Option)
      this.maxtemp = Math.max.apply(null, this.devicetempdata)
      this.mintemp = Math.min.apply(null, this.devicetempdata)
      this.maxhum = Math.max.apply(null, this.devicehumdata)
      this.minhum = Math.min.apply(null, this.devicehumdata)
      this.avgtemp = this.avg(this.devicetempdata)
      this.avghum = this.avg(this.devicehumdata)
    },
    // 初始化
    Init () {
      this.initChart()
      this.initchart()
    }
  }
}
</script>

<style lang="scss" scoped>
.Container {
  display: flex;
  justify-content: center;
  align-items: center;
  flex-direction: column;
  padding: 20px 10px;
  .wrap {
    width: 100%;
    margin: 0 auto;
    height: 300px;
    .analysis {
      display: flex;
      justify-content: space-between;
      align-items: center;
      flex-direction: row;
      margin-bottom: 30px;
    }
  }
}
</style>