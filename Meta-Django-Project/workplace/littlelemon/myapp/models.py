from django.db import models

# Create your models here.
class Home(models.Model):
    home = "home"
class Menu(models.Model):
    name = models.CharField(max_length=200)
    price = models.IntegerField()

    def __str__(self):
        return self.name
class About(models.Model):
    about = "about"