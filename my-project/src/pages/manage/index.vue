<template>
  <div class="wrapper">
    <div class="header">
      <div class="username" v-if="currentUser.admin === '1'">
        管理员:{{ currentUser.username }}
      </div>
      <div class="username" v-else>{{ currentUserList.username }}</div>
      <div class="icon-sex">
        <img src="/static/images/boy.png" v-if="currentUser.sex === '1'" /><img
          src="/static/images/girl.png"
          v-else
        />
      </div>
    </div>
    <div class="manage">
      <div class="userlist" v-if="currentUser.admin === '1'">
        <ul>
          <li v-for="(item, index) in userList" :key="index">
            <p v-if="item[0] !== currentUser.username">{{ item[0] }}</p>
            <div class="btn-control" v-if="item[0] !== currentUser.username">
              <button
                class="btn enable"
                v-if="item[2] == 0"
                @click="enable(item[0])"
              >
                启用
              </button>
              <button class="btn disable" v-else @click="disable(item[0])">
                禁用
              </button>
              <button class="btn delete" @click="remove(item[0])">删除</button>
              <van-dialog id="van-dialog"></van-dialog>
            </div>
          </li>
        </ul>
      </div>

      <div class="userinfo" v-else>
        <ul>
          <li>
            <span class="border-white">设备id</span>
            <span>{{ currentDevice.deviceid }}</span>
          </li>
          <li>
            <span class="border-white">用户名</span>
            <span v-show="!editing">{{ currentUserList.username }}</span>
            <span v-show="editing"
              ><input type="text" v-model="newusername"
            /></span>
          </li>
          <li>
            <span class="border-white">密码</span>
            <span v-show="!editing"> {{ currentUserList.password }}</span>
            <span v-show="editing"
              ><input type="password" v-model="newpassword"
            /></span>
          </li>
          <li>
            <span class="border-white">性别</span>
            <span v-show="!editing">{{
              currentUserList.sex === "1" ? "男" : "女"
            }}</span>
            <span v-show="editing"
              ><div class="sex-radio">
                <van-radio-group
                  direction="horizontal"
                  :value="newsex"
                  @change="onSelectSex"
                >
                  <van-radio name="1" checked-color="#07c160"
                    ><span style="color: #ffffff">男</span></van-radio
                  >
                  <van-radio name="0" checked-color="#07c160"
                    ><span style="color: #ffffff">女</span></van-radio
                  >
                </van-radio-group>
              </div></span
            >
          </li>
        </ul>
        <div>
          <button v-show="!editing" class="editinfo" @click="editinfo">
            <p>{{ editstatus }}</p>
          </button>
          <button v-show="editing" class="editinfo" @click="updateinfo">
            <p>{{ editstatus }}</p>
          </button>
        </div>
      </div>
      <van-toast id="van-toast" />
    </div>
  </div>
</template>
<script>
import { mapState } from "vuex";
import Toast from "@vant/weapp/dist/toast/toast";
import Dialog from "@vant/weapp/dist/dialog/dialog";
export default {
  data() {
    return {
      userList: [],
      currentUserList: [],
      newusername: "",
      newpassword: "",
      newphonenumber: "",
      newsex: 3,
      editing: false,
      editstatus: "编辑",
    };
  },
  computed: {
    ...mapState({
      currentUser: (state) => state.currentUser,
      currentDevice: (state) => state.currentDevice,
    }),
  },
  mounted() {
    wx.setPageStyle({
      style: {
        height: "100vh",
        overflow: "hidden",
        position: "absolute",
      },
    });
    this.$nextTick(() => {
      if (this.currentUser.admin === "1") {
        this.getalluser();
      } else {
        this.getuser();
      }
    });
    // console.log(this.currentUser);
  },
  methods: {
    //获取所有用户信息
    getalluser() {
      const that = this;
      wx.request({
        url: "https://www.nash141.cloud:444/mysql/getalluserinfo.php",
        method: "GET",
        success(res) {
          that.userList = res.data;
        },
      });
    },
    //获取当前用户信息
    getuser() {
      const that = this;
      wx.request({
        url: "https://www.nash141.cloud:444/mysql/getuserinfo.php",
        method: "GET",
        data: {
          phonenumber: that.currentUser.phonenumber,
        },
        success(res) {
          that.currentUserList = res.data;
        },
      });
    },
    //启用用户
    enable(username) {
      const that = this;
      wx.request({
        url: "https://www.nash141.cloud:444/mysql/operateuser.php",
        method: "GET",
        data: {
          username,
          type: "enable",
          adminid: that.currentUser.userid,
          log: that.currentUser.userid + " " + "enable" + " " + username,
          operateid: new Date().getTime().toString(14),
        },
        success() {
          Toast.success("启用成功");
          that.getalluser();
        },
      });
    },
    //禁用用户
    disable(username) {
      const that = this;
      wx.request({
        url: "https://www.nash141.cloud:444/mysql/operateuser.php",
        method: "GET",
        data: {
          username,
          type: "disable",
          adminid: that.currentUser.userid,
          log: that.currentUser.userid + " " + "disable" + " " + username,
          operateid: new Date().getTime().toString(14),
        },
        success() {
          Toast.success("禁用成功");
          that.getalluser();
        },
      });
    },
    //删除用户
    remove(username) {
      const that = this;
      Dialog.confirm({
        title: "警告",
        message: "是否删除该用户？",
        customStyle: "color:#000000",
      })
        .then(() => {
          wx.request({
            url: "https://www.nash141.cloud:444/mysql/operateuser.php",
            method: "GET",
            data: {
              username,
              type: "remove",
              adminid: that.currentUser.userid,
              log: that.currentUser.userid + " " + "remove" + " " + username,
              operateid: new Date().getTime().toString(14),
            },
            success() {
              Toast.success("删除成功");
              that.getalluser();
            },
          });
        })
        .catch(() => {});
    },
    //进入编辑状态
    editinfo() {
      this.editing = !this.editing;
      this.editstatus = "保存";
    },
    //更新用户信息
    updateinfo() {
      const that = this;
      const userinfo = {
        username: this.newusername,
        password: this.newpassword,
        phonenumber: this.currentUserList.phonenumber,
        sex: this.newsex,
      };
      if (
        this.newusername !== "" &&
        this.newpassword !== "" &&
        this.newsex !== 3
      ) {
        wx.request({
          url: `https://www.nash141.cloud:444/mysql/updateuserinfo.php`,
          methods: "POST",
          data: userinfo,
          success() {
            Toast.success("修改成功");
            setTimeout(() => {
              that.getuser();
              that.newusername = "";
              that.newpassword = "";
              that.newsex = 3;
              that.editing = !that.editing;
              that.editstatus = "编辑";
            }, 1000);
          },
        });
      } else {
        Toast.fail("请正确输入信息");
      }
    },
    //获取选择的性别
    onSelectSex(e) {
      this.newsex = e.mp.detail;
    },
  },
};
</script>

<style lang="scss">
.wrapper {
  display: flex;
  flex-direction: column;
  height: 100%;
  .header {
    margin: 10px;
    display: flex;
    align-items: center;
    justify-content: space-around;
    flex-direction: column;
    color: white;
    font-family: HYKaiti;
    background-color: #3d7ef9;
    border-radius: 15px;
    height: 30%;
    .username {
      font-size: 30px;
      margin: 10px 0px;
    }
    .icon-sex img {
      width: 50px;
      height: 50px;
      margin: 10px 0px;
    }
  }
  .manage {
    margin: 0px 10px 10px 10px;
    border-radius: 15px;
    background-color: #3d7ef9;
    color: #ffffff;
    // flex: 1;
    height: 65%;
    .userlist {
      height: 100%;
      overflow: scroll;
      ul {
        margin: 10px 0px;
        height: 380px;
        position: relative;
        overflow-y: scroll;
        overflow: hidden scroll;
        li {
          display: flex;
          align-items: center;
          justify-content: space-between;
          margin: 10px -20px 10px 20px;
          font-size: 20px;
          .btn-control {
            margin: 0px 20px;
            display: flex;
            align-items: center;
            justify-content: space-around;
            .btn {
              margin: 0px 10px 0px 5px;
              font-size: 16px;
              width: 50px;
              height: 30px;
              line-height: 30px;
              color: #ffffff;
              font-size: 10px;
            }
            .enable {
              background-color: #398439;
            }
            .disable {
              background-color: #adadad;
            }
            .delete {
              background-color: #ac2925;
            }
          }
        }
      }
    }
    .userinfo {
      height: 100%;
      ul {
        overflow: scroll;
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: space-around;
        margin-top: 20px;
        li {
          width: 70%;
          display: flex;
          justify-content: space-between;
          flex-direction: column;
          .border-white {
            font-size: 22px;
          }
          span {
            margin: 5px;
            margin-bottom: 0px;
            input {
              color: #ffffff;
              border-bottom: 1px solid #ffffff;
              padding: 3px;
            }
          }
          .sex-radio {
            color: white;
          }
        }
      }
      .editinfo {
        bottom: 20px;
        left: 50%;
        transform: translateX(-30px);
        position: absolute;
        width: 60px;
        height: 30px;
        text-align: center;
        line-height: 30px;
        background-color: #0066ff;
        font-family: SimHei;
        font-size: 16px;
        color: #ffffff;
      }
    }
  }
}
.van-toast--icon {
  min-width: 100px;
  width: unset !important;
}
</style>