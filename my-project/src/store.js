import Vue from 'vue'
import Vuex from 'vuex'
Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    weatherurl: 'https://devapi.qweather.com/v7', // 天气url
    key: 'f507fd5392b841f397d079ec6bb34b94', // key
    loactionurl: 'https://geoapi.qweather.com/v2', // 位置url
    datalist: {
      city: '', // 城市
      area: '', // 区域
      airText: '加载中', // 空气优良
      airValue: '', // 空气指数
      weather: '', // 天气
      weatherAdvice: '' // 天气建议
    },
    currentUser: {
      username: 'Nash', // 当前用户名
      admin: 1, // 是否是管理员
      status: 1, // 是否启用
      sex: 1 // 性别
    },
    loading: true,
    devicetempdata: [],
    devicehumdata: [],
    currenttime: []

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
    },
    // 设置温度数据
    SetDeviceTempData (state, data) {
      state.devicetempdata.push(data)
    },
    // 设置湿度数据
    SetDeviceHumData (state, data) {
      state.devicehumdata.push(data)
    },
    // 设置当前时间
    SetCurrentTime (state, data) {
      state.currenttime.push(data)
    },
    // 设置骨架屏显示
    SetLoading (state, data) {
      state.loading = data
    },
    SetCurrentUser (state, username) {
      state.currentUser.username = username
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
          setTimeout(() => {
            context.commit('SetLoading', false)
          }, 1000)
        }
      })
    }
  }
})
