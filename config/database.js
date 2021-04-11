const database = {
  url: `http://203.195.212.95/login.php`,
  method: 'POST',
  data: {
    username: `${this.inputUserName}`,
    password: `${this.inputUserPassword}`
  },
  header: {
    'content-Type': 'application/x-www-form-urlencoded'
    // "content-Type": "application/json"
  },
  success (res) {
    console.log(res)
  },
  fail (err) {
    console.log(err)
  }
}

export default database
