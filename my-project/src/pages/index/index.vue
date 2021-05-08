 /* eslint-disable */ 

<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <van-skeleton :row="4" :loading="loading">
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
              <div class="data-value" v-show="Status">{{ Temp }}°C</div>
              <div class="data-value" v-show="!Status">已停用</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/humidity.png" alt="" />
            <div class="data-text">
              <div class="data-title">湿度</div>
              <div class="data-value" v-show="Status">{{ Hum }}%</div>
              <div class="data-value" v-show="!Status">已停用</div>
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
              <div class="data-value" v-show="Status">{{ Light }}Lx</div>
              <div class="data-value" v-show="!Status">已停用</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/led.png" alt="" />
            <div class="data-text">
              <div class="data-title">客厅灯</div>
              <switch
                @change="onLedChange"
                :checked="Led"
                :disabled="!Status"
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
                :disabled="!Status"
                class="wx-switch-input"
                color="#3d7ef9"
              />
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/device.png" alt="" />
            <div class="data-text">
              <div class="data-title">状态</div>
              <switch
                @change="onStatusChange"
                :checked="Status"
                class="wx-switch-input"
                color="#3d7ef9"
              />
            </div>
          </div>
        </div>
      </div>
    </div>
    <van-toast id="van-toast" />
  </div>
</template>

<script>
import { connect } from "mqtt/dist/mqtt.js";
import { mapState } from "vuex";
import Toast from "@vant/weapp/dist/toast/toast";
const mqttUrl = "wxs://www.nash141.cloud:8084/mqtt";
// const mqttUrl = "wx://www.nash141.cloud:8083/mqtt";
export default {
  data() {
    return {
      client: {}, // 连接对象
      Temp: 0, // 温度
      Hum: 0, // 湿度
      Light: 0, // 光照度
      Led: 0, // led是否开启
      Beep: 0, // 蜂鸣器是否开启
      Status: true, //设备是否启用
    };
  },
  computed: {
    ...mapState({
      datalist: (state) => state.datalist,
      loading: (state) => state.loading,
      currentDevice: (state) => state.currentDevice,
    }),
  },
  watch: {
    //若设备状态发生变化
    Status() {
      wx.request({
        url: "http://www.nash141.cloud/mysql/changedevicestatus.php",
        method: "GET",
        data: {
          status: this.Status,
          master: this.currentDevice.master,
        },
        success() {
          Toast.success("修改成功");
        },
      });
      //启用状态
      if (this.Status == 1) {
        this.client.on("connect", () => {
          this.client.subscribe(
            `
          ${this.currentDevice.pubtopic}`,
            (err) => {
              if (err) {
                console.log(err);
              }
            }
          );
        });
      } else {
        this.client.on("connect", () => {
          this.client.unsubscribe(
            `
          ${this.currentDevice.pubtopic}`,
            (err) => {
              if (err) {
                console.log(err);
              }
            }
          );
        });
      }
    },
  },
  methods: {
    // 监听led灯
    onLedChange(e) {
      // 开关当前取值
      let sw = e.mp.detail.value;
      if (sw) {
        //开灯
        this.client.publish(
          `
          ${this.currentDevice.subtopic}`,
          '{"target":"LED","value":1}',
          (err) => {
            if (err) {
              console.log(err);
            }
          }
        );
      } else {
        //关灯
        this.client.publish(
          `
          ${this.currentDevice.subtopic}`,
          '{"target":"LED","value":0}',
          (err) => {
            if (err) {
              console.log(err);
            }
          }
        );
      }
    },
    // 监听蜂鸣器
    onBeepChange(e) {
      let sw = e.mp.detail.value;
      if (sw) {
        //打开报警器
        this.client.publish(
          `
          ${this.currentDevice.subtopic}`,
          '{"target":"BEEP","value":1}',
          (err) => {
            if (err) {
              console.log(err);
            }
          }
        );
      } else {
        //关闭报警器
        this.client.publish(
          `
          ${this.currentDevice.subtopic}`,
          '{"target":"BEEP","value":0}',
          (err) => {
            if (err) {
              console.log(err);
            }
          }
        );
      }
    },
    // 监听设备状态
    onStatusChange(e) {
      if (e.mp.detail.value) {
        this.Status = 1;
      } else {
        this.Status = 0;
      }
      console.log(this.Status);
    },
    // 获取数据
    getData() {
      if (this.$store.state.datalist.city === "") {
        this.$store.dispatch("getData");
      }
    },
    // 下拉页面刷新
    refresh() {
      Toast.loading({
        duration: 1000,
        forbidClick: true,
        message: "刷新中...",
        loadingType: "spinner",
      });
      if (wx.startPullDownRefresh) {
        this.$store.dispatch("getData");
        wx.stopPullDownRefresh();
      }
    },
  },
  //页面加载钩子
  created() {
    //连接mqqt伺服器

    // this.client.on("connect", () => {
    //   this.client.subscribe(
    //     '/234/pub',
    //     (err) => {
    //       if (err) {
    //         console.log(err);
    //       }
    //     }
    //   );
    // });
    this.getData();
  },
  //页面展示钩子
  mounted() {
    this.client = connect(mqttUrl);
    this.client.on("connect", () => {
      this.client.subscribe(`${this.currentDevice.pubtopic}`, (err) => {
        if (err) {
          console.log(err);
        }
      });
    });
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
      this.Led = dataFromDevice.Led;
      this.Beep = dataFromDevice.Beep;
      if (dataFromDevice.Temp && dataFromDevice.Hum) {
        this.$store.commit("SetDeviceTempData", this.Temp);
        this.$store.commit("SetDeviceHumData", this.Hum);
        this.$store.commit("SetCurrentTime", time);
      }
    });
  },
  // 页面下拉刷新钩子
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