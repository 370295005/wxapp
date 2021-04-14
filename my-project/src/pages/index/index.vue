 /* eslint-disable */ 

<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <van-skeleton :row="3" :loading="loading">
        <div class="header-title">
          <span>{{ "空气质量:" + datalist.airText }}</span>
          <span>{{ datalist.city }} {{ datalist.area }}</span>
        </div>
        <div class="header-text">
          <span>{{ datalist.airValue }}</span>
          <span>{{ datalist.weather }}</span>
        </div>
        <div class="weather-advice">{{ datalist.weatherAdvice }}</div>
      </van-skeleton>
    </div>
    <div class="body-wrapper">
      <div class="body">
        <div class="data-wrapper">
          <div class="data">
            <img
              class="data-logo"
              src="/static/images/temperature.png"
              alt=""
            />
            <div class="data-text">
              <div class="data-title">温度</div>
              <div class="data-value">{{ Temp }}°C</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/humidity.png" alt="" />
            <div class="data-text">
              <div class="data-title">湿度</div>
              <div class="data-value">{{ Hum }}%</div>
            </div>
          </div>
        </div>
        <div class="data-wrapper">
          <div class="data">
            <img
              class="data-logo"
              src="/static/images/illumination.png"
              alt=""
            />
            <div class="data-text">
              <div class="data-title">光照</div>
              <div class="data-value">{{ Light }}Lx</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/led.png" alt="" />
            <div class="data-text">
              <div class="data-title">客厅灯</div>
              <switch
                @change="onLedChange"
                :checked="Led"
                class="wx-switch-input"
                color="#3d7ef9"
              />
            </div>
          </div>
        </div>
        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/beep.png" alt="" />
            <div class="data-text">
              <div class="data-title">报警器</div>
              <switch
                @change="onBeepChange"
                :checked="Beep"
                class="wx-switch-input"
                color="#3d7ef9"
              />
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { connect } from "mqtt/dist/mqtt.js";
import { mapState } from "vuex";

const mqttUrl = "wxs://www.nash141.cloud:8084/mqtt";
export default {
  data() {
    return {
      client: {},
      Temp: 0, //温度
      Hum: 0, //湿度
      Light: 0, //光照度
      Led: 0, //led是否开启
      Beep: 0, //蜂鸣器是否开启
    };
  },
  computed: {
    ...mapState({
      datalist: (state) => state.datalist,
      day3: (state) => state.analysis.day3,
      loading: (state) => state.loading,
    }),
  },
  watch: {
    loading() {},
  },
  methods: {
    onLedChange(e) {
      //开关当前取值
      let sw = e.mp.detail.value;
      if (sw) {
        //开灯
        this.client.publish("/smart/sub", '{"LED_SW":1}', (err) => {
          if (err) {
            console.log(err);
          }
        });
      } else {
        //关灯
        this.client.publish("/smart/sub", '{"LED_SW":0}', (err) => {
          if (err) {
            console.log(err);
          }
        });
      }
    },
    onBeepChange(e) {
      let sw = e.mp.detail.value;
      if (sw) {
        //打开报警器
        this.client.publish("/smart/sub", '{"BEEP_SW":1}', (err) => {
          if (err) {
            console.log(err);
          }
        });
      } else {
        //关闭报警器
        this.client.publish("/smart/sub", '{"BEEP_SW":0}', (err) => {
          if (err) {
            console.log(err);
          }
        });
      }
    },
    getData() {
      if (this.$store.state.datalist.city === "") {
        this.$store.dispatch("getData");
      }
      console.log(this.loading);
    },
    refresh() {
      //下拉页面刷新
      if (wx.startPullDownRefresh) {
        this.$store.dispatch("getData");
        wx.stopPullDownRefresh();
      }
    },
  },
  onLoad() {
    this.client = connect(mqttUrl);
    this.client.on("connect", () => {
      this.client.subscribe("/smart/sub", (err) => {
        if (err) {
          console.log(err);
        }
      });
    });
    this.getData();
  },
  onShow() {
    //订阅信息
    this.client.on("message", (topic, message) => {
      let date = new Date();
      let sec = date.getSeconds();
      let min = date.getMinutes();
      let time = min + "分" + sec + "秒";
      let dataFromDevice = JSON.parse(message);
      console.log(dataFromDevice);
      this.Temp = dataFromDevice.Temp;
      this.Hum = dataFromDevice.Hum;
      this.Light = dataFromDevice.Light;
      this.Led = dataFromDevice.LED_SW;
      this.Beep = dataFromDevice.BEEP_SW;
      this.$store.commit("SetDeviceTempData", this.Temp);
      this.$store.commit("SetDeviceHumData", this.Hum);
      this.$store.commit("SetCurrentTime", time);
      // this.$store.commit("SetCurrentSec", sec);
      // this.$store.commit("SetCurrentMin", min);
    });
  },
  onPullDownRefresh() {
    this.refresh();
  },
};
</script>

<style lang="scss" scoped>
.wrapper {
  padding: 20px 15px 15px 15px;
  height: 100%;
  .header-wrapper {
    background-color: #3d7ef9;
    border-radius: 20px;
    color: #fff;
    box-shadow: #d6d6d6 0px 0px 5px;
    padding: 15px 30px;
    .header-title {
      display: flex;
      justify-content: space-between;
    }
    .header-text {
      font-size: 32px;
      font-weight: 400;
      display: flex;
      justify-content: space-between;
    }
    .weather-advice {
      margin-top: 20px;
      font-size: 12px;
    }
  }
  .data-wrapper {
    margin-top: 20px;
    display: flex;
    justify-content: space-between;
    .data {
      background-color: #fff;
      width: 150px;
      height: 80px;
      border-radius: 20px;
      display: flex;
      justify-content: space-around;
      align-items: center;
      padding: 0px 8px;
      box-shadow: #d6d6d6 0px 0px 5px;
      .data-logo {
        height: 36px;
        width: 36px;
      }
      .data-text {
        color: #7f7f7f;
        .wx-switch-input {
          //微信内置开关样式
          transform: scale(0.8) !important;
        }
        .data-title {
          text-align: center;
          font: 22px;
        }
        .data-value {
          font-size: 18px;
          font-weight: 600;
        }
      }
    }
  }
}
</style>