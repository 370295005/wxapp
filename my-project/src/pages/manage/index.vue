<template>
  <div class="wrapper">
    <div class="header">
      <div class="username" v-if="currentUser.admin === 1">管理员:{{currentUser.username}}</div>
      <div class="username" v-else>{{currentUser.username}}</div>
      <div class="icon-sex"><img src="/static/images/boy.png"  v-if="currentUser.sex === 1"><img src="/static/images/girl.png" v-else></div>
    </div>
    <div class="manage">
      <div class="userlist" v-if="currentUser.admin === 1">
        <ul>
          <li v-for="(item,index) in userList" :key="index"><p>{{item[0]}}</p>
            <div class="btn-control" v-if="item[0] !== currentUser.username">
              <button class="btn enable" v-if="item[2] == 0" @click="enable(item[0])">启用</button>
              <button class="btn disable" v-else @click="disable(item[0])">禁用</button>
              <button class="btn delete" @click="remove(item[0])">删除</button>
              <van-dialog title="警告" message="是否删除该用户!" use-slot show-cancel-button id="van-dialog" @confirm="remove(item[0])"></van-dialog>
          </div></li>
        </ul>
      </div>
      <div class="userinfo" v-else>userinfo</div>
      <van-toast id="van-toast" />
     
    </div>
  </div>
</template>
<script>
import { mapState } from 'vuex'
import Toast from '@vant/weapp/dist/toast/toast'
import Dialog from '@vant/weapp/dist/dialog/dialog';
export default {
 data(){
  return{
    userList:[]
  }
 },
 computed:{
   ...mapState({
     currentUser:(state)=>state.currentUser
   })
 },
 mounted(){
   console.log(this.currentUser);
   wx.setPageStyle({
     style:{
       height:'100%'
     }
   })
    this.getuser();
   
 },
 methods:{
   getuser(){
     const that = this
     wx.request({
     url:'http://203.195.212.95/mysql/getuser.php',
     method:'GET',
     success(res){
        console.log(res);
        that.userList = res.data
     }
   })
   },
   enable(username){
     const that = this
     wx.request({
       url:'http://203.195.212.95/mysql/enableuser.php',
       method:'GET',
       data:{
         username
       },
       success(){
        Toast.success('启用成功');
        that.getuser()
       }
     })
   },
   disable(username){
     const that = this
     wx.request({
       url:'http://203.195.212.95/mysql/disableuser.php',
       method:'GET',
       data:{
         username
       },
       success(){
        Toast.success('禁用成功');
        that.getuser()
       }
     })
   },
   remove(username){
     const that = this
      Dialog.confirm({
        title:'标题',
        message:'标题'
      })
    .then(() => {
    // on confirm
      wx.request({
       url:'http://203.195.212.95/mysql/removeuser.php',
       method:'GET',
       data:{
         username
       },
       success(){
        Toast.success('删除成功');
        that.getuser()
       }
     })
    })
    .catch(() => {
     // on cancel
   }); 
     
     
   }
 }
}
</script>

<style lang="scss" scoped>
.wrapper{
  display: flex;
  flex-direction: column;
  height: 100%;
  .header{
    margin: 10px;
    display: flex;
    align-items: center;
    justify-content: space-around;
    flex-direction: column;
    color: white;
    font-family:HYKaiti;
    height: 150px;
    background-color: #3d7ef9;
    border-radius: 15px;
    .username{
     font-size: 30px;
    }
    .icon-sex img{
      width: 50px;
      height: 50px;
    }
  }
  .manage{
    margin: 10px;
    border-radius: 15px;
    background-color: #3d7ef9;
    color: #ffffff;
    flex: 1;
    .userlist{
      li{
        display: flex;
        align-items: center;
        justify-content: space-between;
        margin: 10px -20px 10px 20px;
        font-size: 24px;
        .btn-control{
        margin: 0px 20px;
        display: flex;
        align-items: center;
        justify-content: space-around;
        .btn{
        margin: 0px 10px;
        font-size: 16px;
        width: 60px;
        height: 30px;
        line-height: 30px;
        color: #ffffff;
        }
        .enable{
          background-color: #398439;
        }
        .disable{
          background-color: #adadad;
        }
        .delete{
          background-color: #ac2925;
        }
      }
      }
      
    }
  }
}

</style>