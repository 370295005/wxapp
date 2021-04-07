import Vue from 'vue'
import App from './App'
import Vuex from 'vuex'
Vue.config.productionTip = false
App.mpType = 'app'
Vue.use(Vuex)
const app = new Vue(App)
app.$mount()
