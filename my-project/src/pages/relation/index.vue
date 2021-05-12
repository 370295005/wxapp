<template>
  <div>
    <div class="header">
      <div class="header-title">请关联设备</div>
      <div class="header-info">Please Relation Device</div>
    </div>
    <div class="body">
      <div class="login-form">
        <p>设备id</p>
        <van-field placeholder="请输入设备id" @change="onInputDeviceId" :value="deviceid"/>
        <p>设备订阅地址</p>
        <van-field disabled="true" :value="deviceid + '/sub'" />
        <p>设备发布地址</p>
        <van-field disabled="true" :value="deviceid + '/pub'" />
      </div>
      <div><button class="confirm" @click="relationdevice">确定</button></div>
    </div>
    <van-toast id="van-toast" />
  </div>
</template>


<script>
import { mapState } from "vuex";
import Toast from "@vant/weapp/dist/toast/toast";
export default {
  data() {
    return {
      deviceid: "",
      DeviceSub: "",
      DevicePub: "",
    };
  },
  computed: {
    ...mapState({
      currentUser: (state) => state.currentUser,
    }),
  },
  watch: {
    deviceid() {
      this.DeviceSub = "/" + this.deviceid + "/sub";
      this.DevicePub = "/" + this.deviceid + "/pub";
    },
  },
  mounted() {},
  methods: {
    onInputDeviceId(e) {
      this.deviceid = e.mp.detail;
    },
    relationdevice() {
     const that = this;
      if (this.deviceid.trim() == "") {
        Toast.fail("请正确输入设备id");
      } else {
        const params = {
          deviceid: this.deviceid,
          subtopic: this.DeviceSub,
          pubtopic: this.DevicePub,
          status: 1,
          master: this.currentUser.phonenumber,
        };
        console.log(params);
        wx.request({
          url: "https://www.nash141.cloud/mysql/relationdevice.php",
          method: "GET",
          data: params,
          success(res) {
            if (res.data.res == 1) {
              Toast.success("关联成功");
              that.$store.commit("SetCurrentDevice", params);
              setTimeout(() => {
                wx.switchTab({
                  url: "/pages/index/main",
                });
              }, 1000);
            } else {
              Toast.fail("设备id已存在");
              that.deviceid = "";
            }
          },
        });
      }
    },
  },
};
</script>

<style lang="scss">
.header {
  height: 100px;
  padding: 25px 0px;
  background-color: #3d7ef9;
  color: white;
  padding-left: 20px;
  .header-title {
    font-size: 28px;
    font-weight: 500;
  }
  .header-info {
    font-size: 14px;
  }
}
.body {
  border-radius: 15px 15px 0px 0px;
  margin-top: -20px;
  background-color: #fff;
  padding: 20px;
  .login-form {
    position: relative;
    margin: 40px 0px;
  }
  .confirm {
    position: relative;
    display: block;
    margin-left: auto;
    margin-right: auto;
    padding-left: 14px;
    padding-right: 14px;
    box-sizing: border-box;
    font-size: 18px;
    text-align: center;
    text-decoration: none;
    line-height: 2.55555556;
    border-radius: 5px;
    -webkit-tap-highlight-color: transparent;
    overflow: hidden;
    cursor: pointer;
    color: #ffffff;
    background-color: #3d7ef9;
  }
}
.van-toast--icon {
  width: unset !important;
}
</style>