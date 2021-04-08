import Vue from 'vue'
import Vuex from 'vuex'
Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    weatherurl: 'https://devapi.qweather.com/v7', // 天气url
    key: 'f507fd5392b841f397d079ec6bb34b94', // 位置url
    loactionurl: 'https://geoapi.qweather.com/v2', // key
    datalist: {
      city: '', // 城市
      area: '', // 区域
      airText: '加载中', // 空气优良
      airValue: '', // 空气指数
      weather: '', // 天气
      weatherAdvice: '' // 天气建议
    }
  },
  mutations: {
    // 设置位置信息
    SetLocation (state, data) {
      state.datalist.city = data.adm2
      state.datalist.area = data.name
    },
    // 设置天气数据
    SetWeatherData (state, data) {
      state.datalist.weather = data.text
    },
    // 设置天气生活指数
    SetWeatherLifeIndex (state, data) {
      state.datalist.weatherAdvice = data.text
    },
    // 设置天气质量
    SetAirQuality (state, data) {
      state.datalist.airValue = data.aqi
      state.datalist.airText = data.category
    }
  },
  actions: {
    getData (context) {
    // 获取数据
      wx.getLocation({
      // wgs84获取gps坐标
        type: 'wgs84',
        success (res) {
          console.log(res)
          const latitude = res.latitude // 纬度
          const longitude = res.longitude // 经度
          wx.request({
            url: `${context.state.loactionurl}/city/lookup?number=1&location=${longitude},${latitude}&key=${context.state.key}`,
            success (res) {
              console.log('地理位置', res.data.location[0])
              context.commit('SetLocation', res.data.location[0])
            }
          })
          wx.request({
            url: `${context.state.weatherurl}/weather/now?location=${longitude},${latitude}&key=${context.state.key}`,
            success (res) {
              console.log('天气数据', res.data.now)
              context.commit('SetWeatherData', res.data.now)
            }
          })
          wx.request({
            url: `${context.state.weatherurl}/indices/1d?type=1&location=${longitude},${latitude}&key=${context.state.key}`,
            success (res) {
              console.log('天气生活指数', res.data.daily[0])
              context.commit('SetWeatherLifeIndex', res.data.daily[0])
            }
          })
          wx.request({
            url: `${context.state.weatherurl}/air/now?location=${longitude},${latitude}&key=${context.state.key}`,
            success (res) {
              console.log('空气质量', res.data.now)
              context.commit('SetAirQuality', res.data.now)
            }
          })
        }
      })
    }
  }
})
