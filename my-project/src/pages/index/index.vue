 /* eslint-disable */ 
<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <div class="header-title">
        <span>{{ "空气质量:" + airText }}</span>
        <span>{{ city }} {{ area }}</span>
      </div>
      <div class="header-text">
        <span>{{ airValue }}</span>
        <span>{{ weather }}</span>
      </div>
      <div class="weather-advice">{{ weatherAdvice }}</div>
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
    <!-- <div class="bottom">--------123--------</div> -->
  </div>
</template>

<script>
import { connect } from "mqtt/dist/mqtt.js";
const mqttUrl = "wxs://www.nash141.cloud:8084/mqtt";
export default {
  data() {
    return {
      client: {},
      Temp: 0, //温度
      Hum: 0, //湿度
      Light: 0, //光照度
      Led: false, //led是否开启
      Beep: false, //蜂鸣器是否开启
      city: "", //城市
      area: "", //区域
      airText: "", //空气优良
      airValue: "", //空气指数
      weather: "", //天气
      weatherAdvice: "", //天气建议
    };
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
  },
  onShow() {
    const that = this;
    this.client = connect(mqttUrl);
    // console.log(that.client);
    this.client.on("connect", () => {
      this.client.subscribe("/nash", (err) => {
        if (err) {
          console.log(err);
        }
      });
    });
    //订阅信息
    this.client.on("message", (topic, message) => {
      console.log(topic);
      let dataFromDevice = JSON.parse(message);
      console.log(dataFromDevice);
      this.Temp = dataFromDevice.Temp;
      this.Hum = dataFromDevice.Hum;
      this.Light = dataFromDevice.Light;
      this.Led = dataFromDevice.Led;
      this.Beep = dataFromDevice.Beep;
    });

    wx.getLocation({
      //wgs84获取gps坐标
      type: "wgs84",
      success(res) {
        console.log(res);
        const latitude = res.latitude; //纬度
        const longitude = res.longitude; //经度
        const key = "f507fd5392b841f397d079ec6bb34b94";
        //天气url
        const weatherurl = "https://devapi.qweather.com/v7";
        //位置url
        const loactionurl = "https://geoapi.qweather.com/v2";
        wx.request({
          url: `${loactionurl}/city/lookup?number=1&location=${longitude},${latitude}&key=${key}`,
          success(res) {
            console.log("地理位置", res.data.location[0]);
            that.city = res.data.location[0].adm2;
            that.area = res.data.location[0].name;
          },
        });
        wx.request({
          url: `${weatherurl}/weather/now?location=${longitude},${latitude}&key=${key}`,
          success(res) {
            console.log("天气数据", res.data.now);
            that.weather = res.data.now.text;
          },
        });
        wx.request({
          url: `${weatherurl}/indices/1d?type=1&location=${longitude},${latitude}&key=${key}`,
          success(res) {
            console.log("天气生活指数", res.data.daily[0]);
            that.weatherAdvice = res.data.daily[0].text;
          },
        });
        wx.request({
          url: `${weatherurl}/air/now?location=${longitude},${latitude}&key=${key}`,
          success(res) {
            console.log("空气质量", res.data.now);
            that.airValue = res.data.now.aqi;
            that.airText = res.data.now.category;
          },
        });
      },
    });
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
  // .bottom{
  //   text-align: center;
  //   bottom: 0px;
  // }
}
//微信内置开关样式
</style>