                $(function (){ 
                    $.ajax({
                              type: 'GET',
                              url: '/ajax',
                              success: function(data) {
                                console.log('success', data);
                                //$.each(data.value, function(i, item))
                                eval("var value = " +data);
                                console.log('success', value.value);
                              }
                            });
                });
  