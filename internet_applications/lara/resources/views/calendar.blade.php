@extends('layouts.tastytemplate')
@section('title')
    <title>Calendar | Tasty recipes</title>
@stop
@section('additional_head')
    {{ Html::style("css/calendar.css") }} <!-- external reset.css -->
@stop
@section('middle_section')
        <!-- middle section -->
        <div id="calendar">
            <div class="month">
                <ul>
                    <li class="prev">prev</li>
                    <li class="next">next</li>
                    <li style="text-align:center">September</li>
                </ul>
            </div>
            <ul class="weekdays">
                <li>Mo</li>
                <li>Tu</li>
                <li>We</li>
                <li>Th</li>
                <li>Fr</li>
                <li>Sa</li>
                <li>Su</li>
            </ul>
            <ul class="days">
                <li></li>
                <li></li>
                <li></li>
                <li>1</li>
                <li>2</li>
                <li>3</li>
                <li>4</li>
                <li>5</li>
                <li>6</li>
                <li>7</li>
                <li>8</li>
                <li>9</li>
                <li>10</li>
                <li>11</li>
                <li>12</li>
                <li><p>13</p><a href="recipes/meatballs">{{ HTML::image('images/meatballs.png', 'meatballs image') }}</a></li> <!-- Use p tag so the number will stay on top of the image -->
                <li>14</li>
                <li>15</li>
                <li>16</li>
                <li>17</li>
                <li>18</li>
                <li>19</li>
                <li>20</li>
                <li>21</li>
                <li>22</li>
                <li>23</li>
                <li>24</li>
                <li>25</li>
                <li><p>26</p><a href="recipes/pancakes">{{ HTML::image('images/pancakes.png', 'pancakes image') }}</a></li>
                <li>27</li>
                <li>28</li>
                <li>29</li>
                <li>30</li>
            </ul>

        </div>
@stop    
